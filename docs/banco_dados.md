# Banco de dados

## Objetivo do armazenamento

O armazenamento em MySQL registra as mensagens MQTT recebidas para permitir consulta posterior das leituras e estados publicados pelo ESP32. O objetivo e demonstrar persistencia local simples, sem criar backend web ou arquitetura adicional.

## Dependencias

As dependencias Python estao em `storage/requirements.txt`:

- `paho-mqtt`
- `mysql-connector-python`
- `python-dotenv`

## Variaveis de ambiente

O arquivo local `storage/.env` deve ser criado a partir de `storage/.env.example`.

| Variavel | Uso |
|---|---|
| `MYSQL_HOST` | Host do MySQL |
| `MYSQL_PORT` | Porta do MySQL |
| `MYSQL_DATABASE` | Nome do banco |
| `MYSQL_USER` | Usuario do banco |
| `MYSQL_PASSWORD` | Senha do banco |
| `MQTT_BROKER` | Broker MQTT |
| `MQTT_PORT` | Porta MQTT |
| `MQTT_CLIENT_ID` | Identificador do cliente Python |

## Schema

Arquivo: `storage/sql/schema.sql`

Tabela: `leituras_caixa`

| Campo | Tipo | Descricao |
|---|---|---|
| `id` | `INT AUTO_INCREMENT PRIMARY KEY` | Identificador da leitura |
| `topico` | `VARCHAR(128) NOT NULL` | Topico MQTT recebido |
| `valor` | `VARCHAR(128) NOT NULL` | Payload recebido |
| `criado_em` | `TIMESTAMP DEFAULT CURRENT_TIMESTAMP` | Data e hora do registro |

## Como executar

Criar banco e tabela:

```bash
mysql -u root -p < storage/sql/schema.sql
```

Instalar dependencias e executar:

```bash
cd storage
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
python src/mqtt_to_mysql.py
```

## Como visualizar no MySQL Workbench

1. Abrir uma conexao local no MySQL Workbench.
2. Selecionar o schema `iot_caixa_termica`.
3. Executar:

```sql
SELECT id, topico, valor, criado_em
FROM leituras_caixa
ORDER BY id DESC;
```

As capturas de tela devem ser produzidas apenas apos execucao real.
