# Relatorio final

## Capa simples

Titulo: Caixa Termica IoT com ESP32 e MQTT para monitoramento de temperatura, abertura/luminosidade e impacto/vibracao durante transporte de materiais biologicos sensiveis

Disciplina: Internet das Coisas

Integrantes: a preencher

Instituicao: a preencher

Professor(a): a preencher

Data: a preencher

## Introducao

Este trabalho apresenta o desenvolvimento de um prototipo academico de caixa termica IoT baseado em ESP32. O sistema foi planejado para monitorar temperatura, abertura por luminosidade e impacto, vibracao ou inclinacao durante transporte de materiais biologicos sensiveis.

O projeto utiliza sensores conectados ao ESP32, comunicacao MQTT, visualizacao mobile prevista pelo IoT MQTT Panel e armazenamento local em banco de dados MySQL por meio de um script Python.

## Objetivos

### Objetivo geral

Desenvolver um prototipo academico de monitoramento IoT para caixa termica, integrando sensores, atuadores, MQTT, app mobile e banco de dados local.

### Objetivos especificos

- Medir temperatura interna com DS18B20.
- Detectar abertura por variacao de luminosidade com LDR.
- Detectar impacto, vibracao ou inclinacao com MPU6050.
- Publicar dados em topicos MQTT.
- Exibir dados em app mobile.
- Receber comandos remotos para buzzer e reset de alerta.
- Armazenar mensagens MQTT em MySQL.
- Documentar hardware, software e metodologia.

## Motivacao

Materiais biologicos sensiveis podem exigir controle de condicoes durante transporte. Alteracoes de temperatura, abertura indevida e impactos podem indicar risco de degradacao ou falha de procedimento. O prototipo demonstra, em contexto academico, como sensores e comunicacao IoT podem apoiar o acompanhamento dessas condicoes.

Este projeto nao constitui dispositivo medico certificado, produto hospitalar validado ou solucao clinica real.

## Projeto de hardware

Componentes principais:

- ESP32.
- DS18B20 a prova d'agua.
- LDR.
- MPU6050.
- LED.
- Buzzer.
- Display OLED I2C opcional.

Pinagem provisoria:

| Componente | Pino |
|---|---|
| DS18B20 DATA | GPIO 4 |
| LDR | GPIO 34 |
| Buzzer | GPIO 25 |
| LED | GPIO 2 |
| I2C SDA | GPIO 21 |
| I2C SCL | GPIO 22 |

Pontos da montagem fisica: a validar.

## Diagrama eletrico

Diagrama eletrico final: a preencher.

Descricao textual inicial:

```text
ESP32
├── GPIO 4  -> DS18B20 DATA
├── GPIO 34 -> divisor de tensao com LDR
├── GPIO 25 -> buzzer
├── GPIO 2  -> LED
├── GPIO 21 -> SDA MPU6050/OLED opcional
└── GPIO 22 -> SCL MPU6050/OLED opcional
```

## Projeto de software

O software do projeto e dividido em firmware embarcado e armazenamento local.

O firmware, desenvolvido na Arduino IDE, inicializa sensores, conecta o ESP32 ao Wi-Fi, conecta ao broker MQTT, publica leituras e recebe comandos. O script Python assina os topicos MQTT e grava as mensagens recebidas no MySQL.

## Fluxograma

```text
Inicio
  |
  v
Inicializa sensores, pinos e Serial
  |
  v
Conecta Wi-Fi e MQTT
  |
  v
Assina topicos de comando
  |
  v
Le sensores periodicamente
  |
  v
Classifica estado
  |
  +--> NORMAL
  +--> ALERTA_TEMPERATURA
  +--> ALERTA_ABERTURA
  +--> ALERTA_IMPACTO
  |
  v
Publica MQTT e aciona LED/buzzer
  |
  v
Repete ciclo
```

## Linguagem utilizada e justificativa

O firmware utiliza C/C++ no ambiente Arduino IDE por ser a opcao adotada para ESP32 no escopo da disciplina e por possuir bibliotecas prontas para Wi-Fi, MQTT e sensores.

O armazenamento utiliza Python por oferecer integracao simples com MQTT e MySQL usando bibliotecas consolidadas, mantendo o codigo direto e adequado a um prototipo academico.

## Banco de dados

Banco: `iot_caixa_termica`

Tabela: `leituras_caixa`

Campos:

- `id`
- `topico`
- `valor`
- `criado_em`

O banco registra mensagens MQTT em formato textual. Registros reais no MySQL Workbench: a preencher.

## Aplicativo mobile

O app previsto e o IoT MQTT Panel. A configuracao usa o broker `test.mosquitto.org`, porta `1883`, cards para telemetria e botoes para comandos.

Cards previstos:

- Temperatura.
- Luminosidade.
- Aceleracao.
- Tampa.
- Estado.
- Alertas.

Botoes previstos:

- Buzzer ON.
- Buzzer OFF.
- Reset alerta.

Validacao do app configurado: a preencher.

## Comunicacao MQTT

Broker: `test.mosquitto.org`  
Porta: `1883`

Topicos principais:

- `iot/caixa_termica/telemetria/temperatura`
- `iot/caixa_termica/telemetria/luminosidade`
- `iot/caixa_termica/telemetria/aceleracao`
- `iot/caixa_termica/status/tampa`
- `iot/caixa_termica/status/estado`
- `iot/caixa_termica/alerta/temperatura`
- `iot/caixa_termica/alerta/abertura`
- `iot/caixa_termica/alerta/impacto`
- `iot/caixa_termica/comando/buzzer`
- `iot/caixa_termica/comando/reset_alerta`

Testes com MQTTBox: a preencher.

## Metodologia agil

O desenvolvimento foi organizado em sprints resumidas:

1. Definicao do escopo e estrutura.
2. Sensores e atuadores.
3. MQTT.
4. App mobile.
5. Banco de dados.
6. Validacao e relatorio final.

O backlog, as sprints e a retrospectiva estao registrados na pasta `scrum`.

## Resultados obtidos

Resultados de compilacao, leituras reais, recebimento MQTT, gravacao no MySQL e demonstracao pelo app mobile: a preencher.

## Conclusao

O projeto organiza uma base tecnica para demonstrar um prototipo de caixa termica IoT com ESP32, sensores, MQTT, app mobile e armazenamento local. A solucao e adequada ao contexto academico e permite validar conceitos de monitoramento remoto, publicacao de telemetria e persistencia de dados.

Conclusao final apos testes praticos: a preencher.

## Anexos

- Codigo do firmware: `firmware/caixa_termica_esp32/caixa_termica_esp32.ino`.
- Configuracao exemplo do firmware: `firmware/caixa_termica_esp32/config.example.h`.
- Script Python: `storage/src/mqtt_to_mysql.py`.
- Schema SQL: `storage/sql/schema.sql`.
