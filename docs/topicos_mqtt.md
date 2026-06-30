# Topicos MQTT

Broker previsto: `test.mosquitto.org`  
Porta: `1883`

| Topico | Direcao para o ESP32 | Payload | Exemplo | Uso |
|---|---|---|---|---|
| `iot/caixa_termica/telemetria/temperatura` | Publica | Texto numerico em Celsius | `5.80` | Informar temperatura medida pelo DS18B20 |
| `iot/caixa_termica/telemetria/luminosidade` | Publica | Texto numerico analogico | `2450` | Informar leitura do LDR |
| `iot/caixa_termica/telemetria/aceleracao` | Publica | Texto numerico em m/s2 | `10.12` | Informar aceleracao total calculada |
| `iot/caixa_termica/status/tampa` | Publica | Texto | `TAMPA_ABERTA` | Indicar estado estimado da tampa |
| `iot/caixa_termica/status/estado` | Publica | Texto | `NORMAL` | Indicar estado geral do sistema |
| `iot/caixa_termica/alerta/temperatura` | Publica | Texto | `TEMPERATURA_FORA_DA_FAIXA` | Sinalizar alerta de temperatura |
| `iot/caixa_termica/alerta/abertura` | Publica | Texto | `ABERTURA_OU_LUMINOSIDADE_DETECTADA` | Sinalizar alerta de abertura/luminosidade |
| `iot/caixa_termica/alerta/impacto` | Publica | Texto | `IMPACTO_OU_VIBRACAO_DETECTADO` | Sinalizar alerta de impacto/vibracao |
| `iot/caixa_termica/comando/buzzer` | Assina | `ON` ou `OFF` | `OFF` | Controlar buzzer remotamente |
| `iot/caixa_termica/comando/reset_alerta` | Assina | `RESET` | `RESET` | Solicitar reset manual de alerta |

Os payloads foram mantidos em texto simples para facilitar a demonstracao no Serial Monitor, MQTTBox, IoT MQTT Panel e script Python.
