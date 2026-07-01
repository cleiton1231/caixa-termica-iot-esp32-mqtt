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

O arquivo `src/main.cpp` contem a logica de leitura dos sensores e impressao das leituras no Serial Monitor. Nesta etapa, o firmware ainda nao publica MQTT; as leituras foram organizadas para facilitar essa integracao depois.

## Configuracao

Para o teste atual via Serial Monitor, nao e necessario configurar Wi-Fi nem MQTT. Quando a integracao MQTT voltar, crie `include/config.h` a partir de `include/config.example.h` e nao coloque credenciais reais em arquivos versionados.

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

- `paulstoffregen/OneWire`
- `milesburton/DallasTemperature`
- `adafruit/Adafruit MPU6050`
- `adafruit/Adafruit Unified Sensor`

O PlatformIO baixa essas dependencias durante o build.
