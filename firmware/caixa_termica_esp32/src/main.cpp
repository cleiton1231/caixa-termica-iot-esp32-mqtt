#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

#include "config.h"

#ifndef WIFI_SSID
#error "Crie firmware/caixa_termica_esp32/include/config.h a partir de include/config.example.h"
#endif

const int DS18B20_DATA_PIN = 4;
const int LDR_PIN = 34;
const int BUZZER_PIN = 25;
const int LED_PIN = 2;
const int I2C_SDA_PIN = 21;
const int I2C_SCL_PIN = 22;

const float TEMPERATURA_MIN_C = 2.0;
const float TEMPERATURA_MAX_C = 8.0;
const int LIMIAR_LUMINOSIDADE = 2200;
const float LIMIAR_ACELERACAO_MS2 = 16.0;
const float ACELERACAO_GRAVIDADE_MS2 = 9.81;

const unsigned long SENSOR_INTERVAL_MS = 3000;
const unsigned long WIFI_RETRY_INTERVAL_MS = 5000;
const unsigned long MQTT_RETRY_INTERVAL_MS = 5000;

const char *TOPICO_TEMP = "iot/caixa_termica/telemetria/temperatura";
const char *TOPICO_LUZ = "iot/caixa_termica/telemetria/luminosidade";
const char *TOPICO_ACEL = "iot/caixa_termica/telemetria/aceleracao";
const char *TOPICO_TAMPA = "iot/caixa_termica/status/tampa";
const char *TOPICO_ESTADO = "iot/caixa_termica/status/estado";
const char *TOPICO_ALERTA_TEMP = "iot/caixa_termica/alerta/temperatura";
const char *TOPICO_ALERTA_ABERTURA = "iot/caixa_termica/alerta/abertura";
const char *TOPICO_ALERTA_IMPACTO = "iot/caixa_termica/alerta/impacto";
const char *TOPICO_CMD_BUZZER = "iot/caixa_termica/comando/buzzer";
const char *TOPICO_CMD_RESET = "iot/caixa_termica/comando/reset_alerta";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
OneWire oneWire(DS18B20_DATA_PIN);
DallasTemperature ds18b20(&oneWire);
Adafruit_MPU6050 mpu;

bool mpuDisponivel = false;
bool buzzerForcado = false;
bool buzzerSilenciado = false;
unsigned long ultimoCicloSensores = 0;
unsigned long ultimaTentativaWifi = 0;
unsigned long ultimaTentativaMqtt = 0;

enum EstadoSistema {
  NORMAL,
  ALERTA_TEMPERATURA,
  ALERTA_ABERTURA,
  ALERTA_IMPACTO
};

String estadoParaTexto(EstadoSistema estado);
void publicar(const char *topico, const String &payload, bool retido = false);
void aplicarAtuadores(EstadoSistema estado);
void tratarComando(char *topico, byte *payload, unsigned int length);
void conectarWifiSeNecessario();
void conectarMqttSeNecessario();
float lerTemperaturaC();
float lerAceleracaoTotal();
EstadoSistema classificarEstado(float temperaturaC, int luminosidade, float aceleracaoTotal);
void publicarAlertas(EstadoSistema estado);
void cicloSensores();

String estadoParaTexto(EstadoSistema estado) {
  switch (estado) {
    case ALERTA_TEMPERATURA:
      return "ALERTA_TEMPERATURA";
    case ALERTA_ABERTURA:
      return "ALERTA_ABERTURA";
    case ALERTA_IMPACTO:
      return "ALERTA_IMPACTO";
    case NORMAL:
    default:
      return "NORMAL";
  }
}

void publicar(const char *topico, const String &payload, bool retido) {
  if (mqttClient.connected()) {
    mqttClient.publish(topico, payload.c_str(), retido);
  }
}

void aplicarAtuadores(EstadoSistema estado) {
  bool haAlerta = estado != NORMAL;
  digitalWrite(LED_PIN, haAlerta ? HIGH : LOW);

  bool buzzerLigado = buzzerForcado || (haAlerta && !buzzerSilenciado);
  digitalWrite(BUZZER_PIN, buzzerLigado ? HIGH : LOW);
}

void tratarComando(char *topico, byte *payload, unsigned int length) {
  String mensagem;
  for (unsigned int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }
  mensagem.trim();
  mensagem.toUpperCase();

  String topicoRecebido = String(topico);
  Serial.print("Comando MQTT recebido em ");
  Serial.print(topicoRecebido);
  Serial.print(": ");
  Serial.println(mensagem);

  if (topicoRecebido == TOPICO_CMD_BUZZER) {
    if (mensagem == "ON") {
      buzzerForcado = true;
      buzzerSilenciado = false;
    } else if (mensagem == "OFF") {
      buzzerForcado = false;
      buzzerSilenciado = true;
      digitalWrite(BUZZER_PIN, LOW);
    }
  }

  if (topicoRecebido == TOPICO_CMD_RESET && mensagem == "RESET") {
    buzzerForcado = false;
    buzzerSilenciado = false;
    publicar(TOPICO_ESTADO, "RESET_ALERTA");
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void conectarWifiSeNecessario() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  unsigned long agora = millis();
  if (agora - ultimaTentativaWifi < WIFI_RETRY_INTERVAL_MS) {
    return;
  }
  ultimaTentativaWifi = agora;

  Serial.print("Conectando ao Wi-Fi: ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void conectarMqttSeNecessario() {
  if (WiFi.status() != WL_CONNECTED || mqttClient.connected()) {
    return;
  }

  unsigned long agora = millis();
  if (agora - ultimaTentativaMqtt < MQTT_RETRY_INTERVAL_MS) {
    return;
  }
  ultimaTentativaMqtt = agora;

  Serial.print("Conectando ao MQTT em ");
  Serial.println(MQTT_BROKER);

  if (mqttClient.connect(MQTT_CLIENT_ID)) {
    Serial.println("MQTT conectado.");
    mqttClient.subscribe(TOPICO_CMD_BUZZER);
    mqttClient.subscribe(TOPICO_CMD_RESET);
    publicar(TOPICO_ESTADO, "ONLINE", true);
  } else {
    Serial.print("Falha MQTT, codigo=");
    Serial.println(mqttClient.state());
  }
}

float lerTemperaturaC() {
  ds18b20.requestTemperatures();
  return ds18b20.getTempCByIndex(0);
}

float lerAceleracaoTotal() {
  if (!mpuDisponivel) {
    return ACELERACAO_GRAVIDADE_MS2;
  }

  sensors_event_t aceleracao;
  sensors_event_t giro;
  sensors_event_t temperatura;
  mpu.getEvent(&aceleracao, &giro, &temperatura);

  float x = aceleracao.acceleration.x;
  float y = aceleracao.acceleration.y;
  float z = aceleracao.acceleration.z;
  return sqrt((x * x) + (y * y) + (z * z));
}

EstadoSistema classificarEstado(float temperaturaC, int luminosidade, float aceleracaoTotal) {
  if (temperaturaC == DEVICE_DISCONNECTED_C || temperaturaC < TEMPERATURA_MIN_C || temperaturaC > TEMPERATURA_MAX_C) {
    return ALERTA_TEMPERATURA;
  }

  if (luminosidade > LIMIAR_LUMINOSIDADE) {
    return ALERTA_ABERTURA;
  }

  if (mpuDisponivel && aceleracaoTotal > LIMIAR_ACELERACAO_MS2) {
    return ALERTA_IMPACTO;
  }

  return NORMAL;
}

void publicarAlertas(EstadoSistema estado) {
  publicar(TOPICO_ALERTA_TEMP, estado == ALERTA_TEMPERATURA ? "TEMPERATURA_FORA_DA_FAIXA" : "OK");
  publicar(TOPICO_ALERTA_ABERTURA, estado == ALERTA_ABERTURA ? "ABERTURA_OU_LUMINOSIDADE_DETECTADA" : "OK");
  publicar(TOPICO_ALERTA_IMPACTO, estado == ALERTA_IMPACTO ? "IMPACTO_OU_VIBRACAO_DETECTADO" : "OK");
}

void cicloSensores() {
  float temperaturaC = lerTemperaturaC();
  int luminosidade = analogRead(LDR_PIN);
  float aceleracaoTotal = lerAceleracaoTotal();
  EstadoSistema estado = classificarEstado(temperaturaC, luminosidade, aceleracaoTotal);
  String tampa = luminosidade > LIMIAR_LUMINOSIDADE ? "TAMPA_ABERTA" : "TAMPA_FECHADA";
  String payloadTemperatura = temperaturaC == DEVICE_DISCONNECTED_C ? "ERRO_SENSOR_TEMPERATURA" : String(temperaturaC, 2);
  String payloadAceleracao = mpuDisponivel ? String(aceleracaoTotal, 2) : "MPU6050_INDISPONIVEL";

  Serial.print("Temperatura C: ");
  Serial.println(payloadTemperatura);
  Serial.print("Luminosidade: ");
  Serial.println(luminosidade);
  Serial.print("Aceleracao total m/s2: ");
  Serial.println(payloadAceleracao);
  Serial.print("Estado: ");
  Serial.println(estadoParaTexto(estado));

  publicar(TOPICO_TEMP, payloadTemperatura);
  publicar(TOPICO_LUZ, String(luminosidade));
  publicar(TOPICO_ACEL, payloadAceleracao);
  publicar(TOPICO_TAMPA, tampa);
  publicar(TOPICO_ESTADO, estadoParaTexto(estado));
  publicarAlertas(estado);
  aplicarAtuadores(estado);
}

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  ds18b20.begin();
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  if (mpu.begin()) {
    mpuDisponivel = true;
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("MPU6050 inicializado.");
  } else {
    mpuDisponivel = false;
    Serial.println("Aviso: MPU6050 nao encontrado. Firmware continuara sem leitura de aceleracao.");
  }

  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(tratarComando);
  conectarWifiSeNecessario();
}

void loop() {
  conectarWifiSeNecessario();
  conectarMqttSeNecessario();
  mqttClient.loop();

  unsigned long agora = millis();
  if (agora - ultimoCicloSensores >= SENSOR_INTERVAL_MS) {
    ultimoCicloSensores = agora;
    cicloSensores();
  }
}
