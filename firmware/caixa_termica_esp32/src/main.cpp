#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

const int DS18B20_DATA_PIN = 4;
const int LDR_DIGITAL_PIN = 27;
const int I2C_SDA_PIN = 21;
const int I2C_SCL_PIN = 22;

const unsigned long SENSOR_INTERVAL_MS = 1000;
const unsigned long MPU_RETRY_INTERVAL_MS = 5000;

OneWire oneWire(DS18B20_DATA_PIN);
DallasTemperature ds18b20(&oneWire);
Adafruit_MPU6050 mpu;

bool mpuDisponivel = false;
unsigned long ultimoCicloSensores = 0;
unsigned long ultimaTentativaMpu = 0;

struct LeiturasSensores {
  float temperatura_c;
  int luminosidade_digital;  // Valor binario do LDR: 0 ou 1.
  float accel_x;
  float accel_y;
  float accel_z;
  float gyro_x;
  float gyro_y;
  float gyro_z;
  float temp_mpu_c;  // Temperatura interna do chip MPU6050, apenas debug.
  bool ds18b20_disponivel;
  bool mpu_disponivel;
};

void iniciarDs18b20();
void iniciarLdrDigital();
void iniciarMpu6050();
int escanearI2c();
void tentarReconectarMpu6050();
LeiturasSensores lerSensores();
void imprimirLeiturasSerial(const LeiturasSensores &leituras);

void iniciarDs18b20() {
  ds18b20.begin();
  Serial.println("DS18B20 iniciado no GPIO 4.");
}

void iniciarLdrDigital() {
  pinMode(LDR_DIGITAL_PIN, INPUT);
  Serial.println("LDR digital iniciado no GPIO 27. Leitura binaria: 0 ou 1.");
}

void iniciarMpu6050() {
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  int dispositivosEncontrados = escanearI2c();
  if (dispositivosEncontrados == 0) {
    Serial.println("Verifique VCC 3V3, GND, SDA GPIO 21 e SCL GPIO 22.");
  }

  // XDA, XCL, AD0 e INT nao sao usados nesta montagem inicial.
  if (mpu.begin(0x68) || mpu.begin(0x69)) {
    mpuDisponivel = true;
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    Serial.println("MPU6050 iniciado em SDA GPIO 21 / SCL GPIO 22.");
  } else {
    mpuDisponivel = false;
    Serial.println("Erro: MPU6050 nao encontrado no barramento I2C.");
  }
}

int escanearI2c() {
  int encontrados = 0;

  Serial.println("Escaneando barramento I2C...");
  for (byte endereco = 1; endereco < 127; endereco++) {
    Wire.beginTransmission(endereco);
    byte erro = Wire.endTransmission();

    if (erro == 0) {
      Serial.print("Dispositivo I2C encontrado em 0x");
      if (endereco < 16) {
        Serial.print("0");
      }
      Serial.println(endereco, HEX);
      encontrados++;
    }
  }

  if (encontrados == 0) {
    Serial.println("Nenhum dispositivo I2C encontrado.");
  }

  return encontrados;
}

void tentarReconectarMpu6050() {
  if (mpuDisponivel) {
    return;
  }

  unsigned long agora = millis();
  if (agora - ultimaTentativaMpu < MPU_RETRY_INTERVAL_MS) {
    return;
  }
  ultimaTentativaMpu = agora;

  Serial.println("Tentando detectar MPU6050 novamente...");
  iniciarMpu6050();
}

LeiturasSensores lerSensores() {
  LeiturasSensores leituras = {};

  ds18b20.requestTemperatures();
  leituras.temperatura_c = ds18b20.getTempCByIndex(0);
  leituras.ds18b20_disponivel = leituras.temperatura_c != DEVICE_DISCONNECTED_C;

  leituras.luminosidade_digital = digitalRead(LDR_DIGITAL_PIN);

  leituras.mpu_disponivel = mpuDisponivel;
  if (mpuDisponivel) {
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    mpu.getEvent(&accel, &gyro, &temp);

    leituras.accel_x = accel.acceleration.x;
    leituras.accel_y = accel.acceleration.y;
    leituras.accel_z = accel.acceleration.z;
    leituras.gyro_x = gyro.gyro.x;
    leituras.gyro_y = gyro.gyro.y;
    leituras.gyro_z = gyro.gyro.z;
    leituras.temp_mpu_c = temp.temperature;
  }

  return leituras;
}

void imprimirLeiturasSerial(const LeiturasSensores &leituras) {
  Serial.println("----- leituras -----");

  Serial.print("temperatura_c: ");
  if (leituras.ds18b20_disponivel) {
    Serial.println(leituras.temperatura_c, 2);
  } else {
    Serial.println("ERRO_DS18B20");
  }

  Serial.print("luminosidade_digital: ");
  Serial.println(leituras.luminosidade_digital);

  if (leituras.mpu_disponivel) {
    Serial.print("accel_x: ");
    Serial.println(leituras.accel_x, 4);
    Serial.print("accel_y: ");
    Serial.println(leituras.accel_y, 4);
    Serial.print("accel_z: ");
    Serial.println(leituras.accel_z, 4);
    Serial.print("gyro_x: ");
    Serial.println(leituras.gyro_x, 4);
    Serial.print("gyro_y: ");
    Serial.println(leituras.gyro_y, 4);
    Serial.print("gyro_z: ");
    Serial.println(leituras.gyro_z, 4);
    Serial.print("temp_mpu_c: ");
    Serial.println(leituras.temp_mpu_c, 2);
  } else {
    Serial.println("accel_x: MPU6050_INDISPONIVEL");
    Serial.println("accel_y: MPU6050_INDISPONIVEL");
    Serial.println("accel_z: MPU6050_INDISPONIVEL");
    Serial.println("gyro_x: MPU6050_INDISPONIVEL");
    Serial.println("gyro_y: MPU6050_INDISPONIVEL");
    Serial.println("gyro_z: MPU6050_INDISPONIVEL");
    Serial.println("temp_mpu_c: MPU6050_INDISPONIVEL");
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("Teste de sensores da caixa termica IoT");
  Serial.println("Alimentacao dos sensores: 3V3 e GND comum.");

  iniciarDs18b20();
  iniciarLdrDigital();
  iniciarMpu6050();
}

void loop() {
  tentarReconectarMpu6050();

  unsigned long agora = millis();
  if (agora - ultimoCicloSensores >= SENSOR_INTERVAL_MS) {
    ultimoCicloSensores = agora;
    LeiturasSensores leituras = lerSensores();
    imprimirLeiturasSerial(leituras);
  }
}
