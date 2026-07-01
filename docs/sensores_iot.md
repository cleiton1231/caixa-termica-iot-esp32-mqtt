# Documentação dos Sensores da Caixa Térmica IoT

Este documento registra os sensores usados no projeto da caixa térmica IoT, suas funções, ligações sugeridas e exemplos básicos de uso com ESP32/Arduino. A ideia é manter uma referência rápida para montagem, testes e integração com o firmware principal.

## Sensores recebidos

| Sensor | Função no projeto | Comunicação / Saída | Tensão | Observação principal |
|---|---|---|---|---|
| MPU-6050 / GY-521 | Impacto, inclinação e vibração da caixa | I2C | 3 V a 5 V | Mede aceleração e rotação em 3 eixos |
| LDR com LM393 | Detecção de abertura por entrada de luz | Digital | normalmente 3,3 V ou 5 V | O firmware atual usa a saída digital no GPIO 27 |
| DS18B20 à prova d’água | Temperatura interna da caixa térmica | 1-Wire | 3 V a 5,5 V | Precisa de resistor pull-up de 4,7 kΩ no barramento de dados |

---

# 1. MPU-6050 / GY-521

## O que é

O MPU-6050 é um sensor inercial com acelerômetro e giroscópio de 3 eixos. Ele mede:

- aceleração nos eixos X, Y e Z;
- rotação nos eixos X, Y e Z;
- inclinação aproximada;
- vibração ou movimentação brusca.

No projeto, ele é usado para detectar se a caixa térmica sofreu impacto, vibração forte, queda ou mudança brusca de inclinação durante o transporte.

## Onde usar no projeto

Use este sensor fixado na estrutura da caixa térmica, em uma área seca e protegida. Ele não deve ficar solto, enterrado ou exposto diretamente à água.

Exemplos de aplicação:

- detectar movimentação brusca da caixa;
- detectar queda ou inclinação anormal;
- registrar vibração durante o transporte;
- validar se a caixa permaneceu estável.

## Pinos do módulo

| Pino MPU-6050 | Função |
|---|---|
| VCC | Alimentação |
| GND | Terra |
| SCL | Clock I2C |
| SDA | Dados I2C |
| XDA | I2C auxiliar, normalmente não usado |
| XCL | I2C auxiliar, normalmente não usado |
| AD0 | Seleção de endereço I2C |
| INT | Interrupção, opcional |

## Ligação com ESP32

Sugestão usando o barramento I2C padrão do ESP32:

| MPU-6050 | ESP32 |
|---|---|
| VCC | 3V3 |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |
| AD0 | GND ou desconectado |
| INT | opcional |

Use 3,3 V no ESP32. Embora muitos módulos GY-521 aceitem 5 V, trabalhar em 3,3 V reduz risco de incompatibilidade lógica.

## Ligação com Arduino Uno

| MPU-6050 | Arduino Uno |
|---|---|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |
| AD0 | GND ou desconectado |
| INT | opcional |

## Endereço I2C

Normalmente:

```text
0x68
```

Se o pino `AD0` for ligado em VCC, pode mudar para:

```text
0x69
```

## Bibliotecas úteis

Para Arduino IDE / PlatformIO:

```text
Adafruit MPU6050
Adafruit Unified Sensor
Wire
```

## Exemplo básico para ESP32 / Arduino

```cpp
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
    Serial.begin(115200);

    if (!mpu.begin()) {
        Serial.println("Erro: MPU-6050 nao encontrado");
        while (true) {
            delay(1000);
        }
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    Serial.println("MPU-6050 iniciado");
}

void loop() {
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;

    mpu.getEvent(&accel, &gyro, &temp);

    Serial.print("Accel X: ");
    Serial.print(accel.acceleration.x);
    Serial.print(" Y: ");
    Serial.print(accel.acceleration.y);
    Serial.print(" Z: ");
    Serial.println(accel.acceleration.z);

    Serial.print("Gyro X: ");
    Serial.print(gyro.gyro.x);
    Serial.print(" Y: ");
    Serial.print(gyro.gyro.y);
    Serial.print(" Z: ");
    Serial.println(gyro.gyro.z);

    delay(1000);
}
```

## Dados que vale salvar/enviar

Para MQTT ou banco de dados, use nomes claros:

```json
{
  "sensor": "mpu6050",
  "accel_x": 0.12,
  "accel_y": 0.05,
  "accel_z": 9.81,
  "gyro_x": 0.01,
  "gyro_y": 0.00,
  "gyro_z": -0.02
}
```

## Cuidados

Não use o MPU-6050 como sensor de umidade ou temperatura externa. Ele até possui sensor de temperatura interno, mas isso mede a temperatura aproximada do chip, não do ambiente de forma confiável.

---

# 2. Sensor de Luminosidade LDR com LM393

## O que é

O LDR é um resistor dependente de luz. Quando a luminosidade aumenta, sua resistência diminui. O módulo com LM393 compara essa leitura com um limite ajustado no trimpot e gera uma saída digital.

No firmware atual, ele é tratado apenas como sensor digital. A leitura é binária: `0` ou `1`.

## Onde usar no projeto

Use este sensor em uma posição interna que receba luz quando a tampa for aberta. Ele deve ficar protegido contra umidade e sem contato direto com gelo, água ou condensação.

Exemplos de aplicação:

- detectar abertura da tampa por entrada de luz;
- registrar leituras de caixa fechada e aberta;
- acionar alerta quando a luminosidade passar do limiar definido;
- comparar luminosidade com temperatura e impacto.

## Pinos comuns do módulo

Alguns módulos têm 3 pinos, outros têm 4 pinos. Nesta etapa, use apenas a saída digital `DO` ou `OUT`.

### Módulo com 3 pinos

| Pino LDR | Função |
|---|---|
| VCC | Alimentação |
| GND | Terra |
| DO / OUT | Saída digital |

### Módulo com 4 pinos

| Pino LDR | Função |
|---|---|
| VCC | Alimentação |
| GND | Terra |
| DO | Saída digital |
| AO | Saída analógica, não usada nesta etapa |

## Ligação com ESP32

### Usando saída digital

| LDR | ESP32 |
|---|---|
| VCC | 3V3 |
| GND | GND |
| DO | GPIO 27 |

Use alimentação em 3,3 V no ESP32. Se alimentar o módulo com 5 V, a saída digital pode ir para 5 V e isso não é seguro para GPIO do ESP32.

Esta é a ligação usada pelo firmware principal em `firmware/caixa_termica_esp32/src/main.cpp`.

## Ligação com Arduino Uno

| LDR | Arduino Uno |
|---|---|
| VCC | 5V |
| GND | GND |
| DO | D7 |
| AO | não usado nesta etapa |

## Ajuste do trimpot

O trimpot define o limiar da saída digital.

Procedimento simples:

1. Ligue o módulo.
2. Abra o monitor serial.
3. Aponte o sensor para luz e sombra.
4. Gire o trimpot até a saída digital mudar no ponto desejado.
5. Fixe esse ajuste para o teste.

A saída digital não mede intensidade com precisão. Ela só diz se passou ou não de um limite.

## Exemplo com saída digital

```cpp
const int PIN_LDR_DIGITAL = 27;

void setup() {
    Serial.begin(115200);
    pinMode(PIN_LDR_DIGITAL, INPUT);
}

void loop() {
    int estado = digitalRead(PIN_LDR_DIGITAL);

    Serial.print("LDR digital: ");
    Serial.println(estado);

    delay(1000);
}
```

Dependendo do módulo e do ajuste do trimpot, `0` pode significar claro ou escuro. Teste na prática e documente no código.

## Dados que vale salvar/enviar

```json
{
  "sensor": "ldr",
  "luminosidade_digital": 1
}
```

## Cuidados

A saída digital do LDR é boa para decisão simples de abertura/fechamento. Ela não mede intensidade de luz; só indica se passou ou não do limiar ajustado no trimpot.

---

# 3. Sensor de Temperatura DS18B20 à prova d’água

## O que é

O DS18B20 é um sensor digital de temperatura que usa protocolo 1-Wire. A versão à prova d’água vem encapsulada em aço inoxidável e cabo isolado.

No projeto, ele mede a temperatura interna da caixa térmica. A versão encapsulada é adequada para ambiente úmido, desde que o cabo e a vedação sejam preservados.

## Onde usar no projeto

Use para medir:

- temperatura interna da caixa térmica;
- temperatura próxima ao material transportado;
- temperatura em contato com superfície fria ou úmida, desde que a vedação esteja íntegra;
- variação de temperatura durante a simulação de transporte.

Não force o cabo nem dobre perto da ponta metálica. A vedação é boa, mas não é indestrutível.

## Fios comuns

A maioria dos DS18B20 impermeáveis segue este padrão:

| Fio | Função |
|---|---|
| Vermelho | VCC |
| Preto | GND |
| Amarelo ou branco | DATA |

Confirme visualmente no seu sensor. Alguns clones podem variar.

## Ligação com ESP32

| DS18B20 | ESP32 |
|---|---|
| Vermelho / VCC | 3V3 |
| Preto / GND | GND |
| Amarelo ou branco / DATA | GPIO 4 |

Além disso, coloque um resistor de `4,7 kΩ` entre `DATA` e `VCC`.

Esquema textual:

```text
ESP32 3V3  ---- VCC do DS18B20
ESP32 GND  ---- GND do DS18B20
ESP32 GPIO4 ---- DATA do DS18B20

Resistor 4,7 kΩ entre DATA e 3V3
```

## Ligação com Arduino Uno

| DS18B20 | Arduino Uno |
|---|---|
| Vermelho / VCC | 5V |
| Preto / GND | GND |
| Amarelo ou branco / DATA | D2 |

Coloque resistor de `4,7 kΩ` entre `DATA` e `5V`.

## Bibliotecas úteis

Para Arduino IDE / PlatformIO:

```text
OneWire
DallasTemperature
```

## Exemplo básico para ESP32 / Arduino

```cpp
#include <OneWire.h>
#include <DallasTemperature.h>

const int PIN_DS18B20 = 4;

OneWire oneWire(PIN_DS18B20);
DallasTemperature sensors(&oneWire);

void setup() {
    Serial.begin(115200);
    sensors.begin();
}

void loop() {
    sensors.requestTemperatures();

    float temperatura = sensors.getTempCByIndex(0);

    if (temperatura == DEVICE_DISCONNECTED_C) {
        Serial.println("Erro: DS18B20 desconectado");
    } else {
        Serial.print("Temperatura: ");
        Serial.print(temperatura);
        Serial.println(" C");
    }

    delay(1000);
}
```

## Dados que vale salvar/enviar

```json
{
  "sensor": "ds18b20",
  "temperatura_c": 24.62
}
```

## Cuidados

O DS18B20 precisa do resistor pull-up. Sem ele, a leitura pode falhar, oscilar ou retornar valores inválidos.

Se a leitura aparecer como `-127 C`, normalmente significa sensor desconectado, resistor ausente, fio errado ou pino errado.

---

# 4. Sugestão de pinagem geral para ESP32

Esta pinagem evita conflito entre sensores e mantém o projeto organizado.

| Função | Sensor | GPIO ESP32 |
|---|---|---|
| SDA I2C | MPU-6050 | GPIO 21 |
| SCL I2C | MPU-6050 | GPIO 22 |
| DATA 1-Wire | DS18B20 | GPIO 4 |
| Digital OUT | LDR | GPIO 27 |

Resumo:

```text
MPU-6050:
VCC -> 3V3
GND -> GND
SDA -> GPIO 21
SCL -> GPIO 22

DS18B20:
VCC -> 3V3
GND -> GND
DATA -> GPIO 4
Resistor 4,7 kΩ entre DATA e 3V3

LDR:
VCC -> 3V3
GND -> GND
DO -> GPIO 27
```

---

# 5. Estrutura sugerida no projeto

Organização atual do projeto:

```text
Iot_trabalho/
├── docs/
│   ├── sensores_iot.md
│   ├── hardware.md
│   ├── topicos_mqtt.md
│   └── ...
├── firmware/
│   └── caixa_termica_esp32/
│       ├── src/
│       │   └── main.cpp
│       ├── include/
│       │   ├── config.example.h
│       │   └── config.h
│       └── platformio.ini
├── storage/
│   ├── src/
│   │   ├── mqtt_to_mysql.py
│   │   └── mqtt_subscriber.py
│   └── sql/
│       └── schema.sql
└── README.md
```

Este arquivo deve ficar em:

```text
docs/sensores_iot.md
```

## O que colocar no README principal

No `README.md` principal, coloque só um resumo e aponte para esta documentação:

```markdown
## Sensores

O projeto usa três sensores principais:

- MPU-6050 para movimento, inclinação e vibração.
- LDR para detecção de luminosidade.
- DS18B20 à prova d’água para temperatura.

A documentação de ligação, pinagem e exemplos está em:

[docs/sensores_iot.md](docs/sensores_iot.md)
```

---

# 6. Ordem recomendada de teste

Teste um sensor por vez. Não monte tudo de uma vez, porque isso dificulta debug.

## Etapa 1: testar DS18B20

Motivo: é o sensor mais importante para ambiente úmido e exige resistor pull-up.

Validar:

```text
temperatura aparece no serial
não retorna -127 C
leitura muda ao tocar na ponta metálica
```

## Etapa 2: testar LDR

Validar:

```text
valor digital muda com caixa fechada e tampa aberta
valor lido fica em 0 ou 1
trimpot ajusta o ponto de mudança da saída digital
```

## Etapa 3: testar MPU-6050

Validar:

```text
I2C detecta endereço 0x68
aceleração muda ao inclinar o módulo
giroscópio muda ao girar o módulo
```

---

# 7. Payloads MQTT usados no projeto

O firmware atual publica os dados em tópicos MQTT separados, com payloads em texto simples. Isso facilita a demonstração no Serial Monitor, IoT MQTT Panel e scripts Python.

| Tópico | Exemplo de payload | Sensor / estado |
|---|---|---|
| `iot/caixa_termica/telemetria/temperatura` | `5.80` | DS18B20 |
| `iot/caixa_termica/telemetria/luminosidade` | `1` | LDR digital |
| `iot/caixa_termica/telemetria/aceleracao` | `10.12` | MPU6050, aceleração total |
| `iot/caixa_termica/status/tampa` | `TAMPA_ABERTA` | Estado estimado pela luminosidade |
| `iot/caixa_termica/status/estado` | `NORMAL` | Estado geral classificado pelo firmware |

Se o projeto evoluir para payload JSON unificado, um formato possível seria:

```json
{
  "device_id": "caixa-termica-esp32",
  "temperatura_c": 5.8,
  "luminosidade_digital": 1,
  "aceleracao_ms2": 10.12,
  "tampa": "TAMPA_ABERTA",
  "estado": "ALERTA_ABERTURA"
}
```

---

# 8. Checklist de montagem

Antes de ligar:

- conferir VCC e GND de todos os módulos;
- usar 3,3 V nos sensores ligados ao ESP32;
- colocar resistor de 4,7 kΩ no DS18B20;
- não ligar saída 5 V em GPIO do ESP32;
- testar um sensor por vez;
- conferir `git status` antes de commitar alterações na documentação.

Depois de testar:

```bash
git add docs/sensores_iot.md
git commit -m "Documenta sensores do projeto IoT"
git push
```
