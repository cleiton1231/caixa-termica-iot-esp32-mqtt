# App mobile

## App previsto

O app previsto para demonstracao e o IoT MQTT Panel. Ele permite configurar conexao com broker MQTT, criar cards de visualizacao e botoes de publicacao sem desenvolver um app proprio.

## Broker

| Campo | Valor |
|---|---|
| Broker | `test.mosquitto.org` |
| Porta | `1883` |
| Usuario | vazio |
| Senha | vazia |
| TLS | desativado para a demonstracao basica |

## Cards sugeridos

| Card | Topico | Tipo esperado |
|---|---|---|
| Temperatura | `iot/caixa_termica/telemetria/temperatura` | Valor numerico/texto |
| Luminosidade | `iot/caixa_termica/telemetria/luminosidade` | Valor numerico/texto |
| Aceleracao | `iot/caixa_termica/telemetria/aceleracao` | Valor numerico/texto |
| Tampa | `iot/caixa_termica/status/tampa` | Texto |
| Estado | `iot/caixa_termica/status/estado` | Texto |
| Alerta temperatura | `iot/caixa_termica/alerta/temperatura` | Texto |
| Alerta abertura | `iot/caixa_termica/alerta/abertura` | Texto |
| Alerta impacto | `iot/caixa_termica/alerta/impacto` | Texto |

## Botoes sugeridos

| Botao | Topico | Payload |
|---|---|---|
| Buzzer ON | `iot/caixa_termica/comando/buzzer` | `ON` |
| Buzzer OFF | `iot/caixa_termica/comando/buzzer` | `OFF` |
| Reset alerta | `iot/caixa_termica/comando/reset_alerta` | `RESET` |

## Evidencias a registrar

- Tela do app conectado ao broker.
- Cards recebendo valores reais.
- Botoes enviando comandos ao ESP32.
- Serial Monitor mostrando recebimento dos comandos.

As imagens devem ser capturadas apenas durante a demonstracao real.
