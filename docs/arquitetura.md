# Arquitetura

## Fluxo geral

```text
DS18B20 + LDR + MPU6050
        |
        v
ESP32 com firmware Arduino
        |
        | Wi-Fi
        v
Broker MQTT test.mosquitto.org:1883
        |
        +--> IoT MQTT Panel
        |
        +--> Python Paho MQTT
                    |
                    v
                  MySQL
```

## Responsabilidade de cada camada

| Camada | Responsabilidade |
|---|---|
| Sensores | Coletar temperatura, luminosidade e aceleracao |
| ESP32 | Ler sensores, classificar estado, acionar atuadores e publicar MQTT |
| Broker MQTT | Intermediar publicacoes e assinaturas entre ESP32, app e Python |
| App mobile | Exibir leituras e enviar comandos simples |
| Script Python | Assinar topicos e persistir mensagens recebidas |
| MySQL | Armazenar historico basico das mensagens MQTT |

## Justificativa da arquitetura

A arquitetura foi escolhida por ser simples, demonstravel e adequada ao escopo academico. O MQTT reduz o acoplamento entre firmware, app mobile e armazenamento, permitindo que cada parte assine apenas os topicos necessarios. O ESP32 concentra a leitura dos sensores e a resposta local por LED e buzzer, enquanto o armazenamento fica externo em um computador com Python e MySQL.

O uso do broker publico facilita a demonstracao sem infraestrutura propria, mas deve ser entendido como recurso didatico.
