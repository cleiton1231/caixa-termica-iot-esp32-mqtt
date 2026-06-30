import os
import signal
import sys

import mysql.connector
from dotenv import load_dotenv
from paho.mqtt import client as mqtt


TOPICO_BASE = "iot/caixa_termica/#"


def carregar_configuracao():
    load_dotenv()
    return {
        "mysql_host": os.getenv("MYSQL_HOST", "localhost"),
        "mysql_port": int(os.getenv("MYSQL_PORT", "3306")),
        "mysql_database": os.getenv("MYSQL_DATABASE", "iot_caixa_termica"),
        "mysql_user": os.getenv("MYSQL_USER", "root"),
        "mysql_password": os.getenv("MYSQL_PASSWORD", ""),
        "mqtt_broker": os.getenv("MQTT_BROKER", "test.mosquitto.org"),
        "mqtt_port": int(os.getenv("MQTT_PORT", "1883")),
        "mqtt_client_id": os.getenv("MQTT_CLIENT_ID", "caixa-termica-storage"),
    }


def conectar_mysql(config):
    return mysql.connector.connect(
        host=config["mysql_host"],
        port=config["mysql_port"],
        database=config["mysql_database"],
        user=config["mysql_user"],
        password=config["mysql_password"],
    )


def inserir_leitura(conexao, topico, valor):
    cursor = conexao.cursor()
    try:
        cursor.execute(
            "INSERT INTO leituras_caixa (topico, valor) VALUES (%s, %s)",
            (topico, valor),
        )
        conexao.commit()
    finally:
        cursor.close()


def main():
    config = carregar_configuracao()
    conexao_mysql = conectar_mysql(config)
    cliente_mqtt = mqtt.Client(client_id=config["mqtt_client_id"])

    def encerrar(_signal=None, _frame=None):
        print("\nEncerrando conexoes...")
        cliente_mqtt.disconnect()
        conexao_mysql.close()
        sys.exit(0)

    def ao_conectar(cliente, _userdata, _flags, codigo_retorno):
        if codigo_retorno == 0:
            print("Conectado ao MQTT.")
            cliente.subscribe(TOPICO_BASE)
            print(f"Assinando: {TOPICO_BASE}")
        else:
            print(f"Falha ao conectar no MQTT. Codigo: {codigo_retorno}")

    def ao_receber_mensagem(_cliente, _userdata, mensagem):
        topico = mensagem.topic
        valor = mensagem.payload.decode("utf-8", errors="replace")
        print(f"{topico} -> {valor}")
        inserir_leitura(conexao_mysql, topico, valor)

    signal.signal(signal.SIGINT, encerrar)
    signal.signal(signal.SIGTERM, encerrar)

    cliente_mqtt.on_connect = ao_conectar
    cliente_mqtt.on_message = ao_receber_mensagem

    print("Conectando ao MySQL e MQTT...")
    cliente_mqtt.connect(config["mqtt_broker"], config["mqtt_port"])
    cliente_mqtt.loop_forever()


if __name__ == "__main__":
    main()
