# Software

## Firmware principal

O firmware principal foi migrado para PlatformIO, mantendo o framework Arduino para ESP32. A estrutura usada e:

```text
firmware/caixa_termica_esp32/
├── platformio.ini
├── include/
│   └── config.example.h
└── src/
    └── main.cpp
```

O arquivo `src/main.cpp` contem a logica de leitura dos sensores, classificacao de estado, publicacao MQTT e recebimento de comandos. Ele inclui `config.h`, que deve ser criado localmente a partir de `include/config.example.h`.

## Configuracao

```bash
cd firmware/caixa_termica_esp32
cp include/config.example.h include/config.h
```

Depois, preencher no `include/config.h`:

- `WIFI_SSID`
- `WIFI_PASSWORD`
- `MQTT_BROKER`
- `MQTT_PORT`
- `MQTT_CLIENT_ID`

Nao colocar credenciais reais em arquivos versionados.

## Build, upload e monitor

Compilar:

```bash
pio run
```

Enviar para o ESP32:

```bash
pio run --target upload
```

Abrir monitor serial:

```bash
pio device monitor
```

## Dependencias PlatformIO

As bibliotecas do firmware principal estao declaradas em `platformio.ini`:

- `knolleary/PubSubClient`
- `paulstoffregen/OneWire`
- `milesburton/DallasTemperature`
- `adafruit/Adafruit MPU6050`
- `adafruit/Adafruit Unified Sensor`

O PlatformIO baixa essas dependencias durante o build.
