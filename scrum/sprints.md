# Sprints

## Sprint 1: definicao do escopo e estrutura

Objetivo: consolidar o tema final, limitar o escopo academico e organizar a estrutura base do repositorio.

Entregas:

- Tema final definido.
- Estrutura de pastas organizada.
- Regras de credenciais e evidencias definidas.
- README inicial da entrega final.

## Sprint 2: sensores e atuadores

Objetivo: implementar a leitura dos sensores e o acionamento local.

Entregas:

- Leitura do DS18B20.
- Leitura analogica do LDR.
- Leitura do MPU6050.
- Acionamento de LED e buzzer.
- Limiar inicial para classificacao de alertas.

## Sprint 3: MQTT

Objetivo: integrar o ESP32 ao broker MQTT e padronizar topicos.

Entregas:

- Conexao Wi-Fi.
- Conexao MQTT com `test.mosquitto.org:1883`.
- Publicacao de telemetria e status.
- Assinatura de comandos para buzzer e reset.
- Tabela de topicos documentada.

## Sprint 4: app mobile

Objetivo: preparar a visualizacao e comandos pelo IoT MQTT Panel.

Entregas:

- Definicao dos cards de temperatura, luminosidade, aceleracao, tampa e estado.
- Definicao dos botoes de buzzer ON/OFF e reset.
- Checklist de evidencias do app.

## Sprint 5: banco de dados

Objetivo: persistir mensagens MQTT em MySQL local.

Entregas:

- Schema SQL inicial.
- Arquivo `.env.example`.
- Dependencias Python.
- Script `mqtt_to_mysql.py`.
- Documentacao de execucao e consulta.

## Sprint 6: evidencias e relatorio final

Objetivo: preparar o material de entrega final.

Entregas:

- Estrutura de pastas para fotos, MQTTBox, Serial Monitor, MySQL Workbench e video.
- Checklist de entrega final.
- Esqueleto do relatorio final.
- Campos pendentes marcados como a preencher ou a validar.
