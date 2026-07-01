# Caixa Termica IoT ESP32 MQTT

Projeto academico de Internet das Coisas para monitoramento de uma caixa termica durante o transporte de materiais biologicos sensiveis. O prototipo usa ESP32, sensores locais, comunicacao MQTT, visualizacao mobile e armazenamento em MySQL por meio de um script Python.

> Aviso: este repositorio descreve um prototipo academico. Ele nao e um dispositivo medico certificado, produto hospitalar validado ou solucao clinica real.

## Resumo tecnico

O ESP32 realiza leituras de temperatura, luminosidade e aceleracao. As leituras sao publicadas em topicos MQTT no broker publico `test.mosquitto.org`, porta `1883`. Um app mobile, previsto com IoT MQTT Panel, pode exibir os dados e enviar comandos simples. Um script Python assina os topicos MQTT e grava as mensagens em uma tabela MySQL.

## Objetivo geral

Construir uma base funcional e documentada para demonstrar um sistema IoT capaz de monitorar condicoes relevantes de transporte em uma caixa termica: temperatura interna, possivel abertura por luminosidade e impacto, vibracao ou inclinacao por acelerometro.

## Componentes usados

- ESP32 com PlatformIO usando framework Arduino.
- Sensor DS18B20 a prova d'agua para temperatura.
- LDR para deteccao de luminosidade/abertura.
- MPU6050 para aceleracao, impacto, vibracao ou inclinacao.
- LED para sinalizacao local.
- Buzzer para alerta sonoro.
- Display OLED I2C opcional como saida local.
- Broker MQTT `test.mosquitto.org:1883`.
- App mobile previsto: IoT MQTT Panel.
- Storage local: Python, Paho MQTT, MySQL Connector, python-dotenv e MySQL.

## Arquitetura

```text
Sensores e atuadores
        |
        v
ESP32 com firmware Arduino
        |
        v
Broker MQTT test.mosquitto.org:1883
        |
        +--> IoT MQTT Panel
        |
        +--> Script Python storage/src/mqtt_to_mysql.py
                    |
                    v
                  MySQL
```

## Como configurar o firmware principal

O firmware principal fica em `firmware/caixa_termica_esp32` e usa PlatformIO com `board = esp32dev` e framework Arduino. A versao atual e de teste dos sensores via Serial Monitor; a integracao MQTT fica para a proxima etapa.

1. Instale o PlatformIO.
2. Compile:

```bash
cd firmware/caixa_termica_esp32
pio run
```

3. Grave no ESP32:

```bash
pio run --target upload
```

4. Abra o monitor serial:

```bash
pio device monitor
```

O arquivo `config.h` sera usado novamente quando Wi-Fi/MQTT forem reativados. Credenciais locais devem continuar fora do Git.

## Como configurar o banco

1. Crie o banco, usuario e permissoes no MariaDB/MySQL local.
2. Aplique o schema:

```bash
mariadb --defaults-extra-file=storage/.my.cnf < storage/sql/schema.sql
```

3. Copie `storage/.env.example` para `storage/.env`, se ainda nao existir.
4. Ajuste usuario, senha, host e porta do MySQL no `.env`.
5. Opcionalmente, use `storage/.my.cnf` local para entrar no banco sem digitar senha manualmente:

```bash
mariadb --defaults-extra-file=storage/.my.cnf
```

Os arquivos `storage/.env` e `storage/.my.cnf` sao locais e ignorados pelo Git.

## Como rodar o script Python

```bash
cd storage
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
python src/mqtt_to_mysql.py
```

O script assina `iot/caixa_termica/#`, imprime topico e payload recebidos e grava cada mensagem na tabela `leituras_caixa`.

Para conferir se o `.env` foi carregado sem imprimir credenciais sensiveis:

```bash
cd storage
python src/check_env.py
```

Para apenas assinar os topicos MQTT e ver as mensagens no terminal, sem gravar no MySQL:

```bash
cd storage
python src/mqtt_subscriber.py
```

Para testar se o banco, o schema e a tabela estao funcionando:

```bash
cd storage
python src/test_mysql.py
```

O teste do MySQL insere uma linha com topico `teste/mysql` e valor `OK` na tabela `leituras_caixa`.

## Simulador local sem sensores

O firmware de simulacao fica em `firmware/caixa_termica_simulador_esp32`. Essa pasta e local de teste e esta ignorada pelo Git. Ele usa Wi-Fi e MQTT reais, mas nao depende de DS18B20, LDR ou MPU6050. O simulador publica nos mesmos topicos do projeto alternando entre `NORMAL`, `ALERTA_TEMPERATURA`, `ALERTA_ABERTURA` e `ALERTA_IMPACTO`.

Para usar:

```bash
cd firmware/caixa_termica_simulador_esp32
pio run
pio run --target upload
pio device monitor
```

O `include/config.h` do simulador tambem e local e ignorado pelo Git.

## Topicos MQTT principais

| Topico | Uso |
|---|---|
| `iot/caixa_termica/telemetria/temperatura` | Temperatura em graus Celsius |
| `iot/caixa_termica/telemetria/luminosidade` | Valor digital do LDR, `0` ou `1` |
| `iot/caixa_termica/telemetria/aceleracao` | Aceleracao total calculada |
| `iot/caixa_termica/status/tampa` | Estado estimado da tampa |
| `iot/caixa_termica/status/estado` | Estado geral do sistema |
| `iot/caixa_termica/alerta/temperatura` | Alerta textual de temperatura |
| `iot/caixa_termica/alerta/abertura` | Alerta textual de abertura/luminosidade |
| `iot/caixa_termica/alerta/impacto` | Alerta textual de impacto |
| `iot/caixa_termica/comando/buzzer` | Comando `ON` ou `OFF` |
| `iot/caixa_termica/comando/reset_alerta` | Comando `RESET` |

## Estrutura do repositorio

```text
.
├── firmware/caixa_termica_esp32/
├── storage/
├── docs/
├── scrum/
└── report/
```

## Como demonstrar o funcionamento

1. Compilar e carregar o firmware no ESP32.
2. Abrir o monitor serial com `pio device monitor` e confirmar conexao Wi-Fi, MQTT e leitura dos sensores.
3. Configurar o IoT MQTT Panel com o broker `test.mosquitto.org` e os topicos do projeto.
4. Executar o script Python e confirmar mensagens recebidas no terminal.
5. Abrir o MySQL Workbench e validar registros na tabela `leituras_caixa`.

Nao adicionar evidencias inventadas. Campos dependentes de testes reais devem permanecer como "a preencher" ou "a validar" ate a demonstracao.
