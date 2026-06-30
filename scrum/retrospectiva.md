# Retrospectiva

## O que funcionou

- A escolha do ESP32 permite Wi-Fi integrado e suporte amplo na Arduino IDE.
- O MQTT simplifica a comunicacao entre firmware, app mobile e script Python.
- A separacao entre firmware, storage, documentacao, Scrum, evidencias e relatorio deixa a entrega mais organizada.
- O uso de payload textual facilita testes com MQTTBox e IoT MQTT Panel.

## Dificuldades

- Os limiares de luminosidade e aceleracao dependem de calibracao na montagem fisica.
- A leitura do LDR pode variar conforme iluminacao ambiente e posicao dentro da caixa.
- O acionamento do buzzer pode exigir transistor, dependendo do modulo utilizado.
- O broker publico pode apresentar instabilidade ou mensagens de outros testes se topicos nao forem suficientemente especificos.
- Evidencias reais ainda precisam ser coletadas na execucao pratica.

## Melhorias futuras

- Adicionar display OLED I2C para mostrar estado localmente.
- Ajustar limiares com base em medicoes reais da caixa fechada, aberta e em movimento.
- Usar topicos com identificador unico da equipe para reduzir colisao em broker publico.
- Persistir leituras em formato estruturado com campos numericos separados.
- Adicionar timestamp do dispositivo quando houver relogio sincronizado.
- Avaliar broker MQTT privado em uma versao futura.

## Pendencias a validar

- Compilacao do firmware na Arduino IDE.
- Funcionamento real do DS18B20.
- Funcionamento real do LDR na caixa.
- Funcionamento real do MPU6050.
- Resposta do LED e buzzer.
- Recebimento de comandos pelo app mobile.
- Gravacao de mensagens no MySQL.
- Coleta de fotos, prints e video da demonstracao.
