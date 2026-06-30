import os
import signal
import sys

from dotenv import load_dotenv
from paho.mqtt import client as mqtt


TOPICO_PADRAO = "iot/caixa_termica/#"


def carregar_configuracao():
    load_dotenv()
    return {
        "mqtt_broker": os.getenv("MQTT_BROKER", "test.mosquitto.org"),
        "mqtt_port": int(os.getenv("MQTT_PORT", "1883")),
        "mqtt_client_id": os.getenv("MQTT_CLIENT_ID", "caixa-termica-subscriber"),
        "mqtt_topic": os.getenv("MQTT_TOPIC", TOPICO_PADRAO),
    }


def main():
    config = carregar_configuracao()
    client = mqtt.Client(client_id=config["mqtt_client_id"])

    def encerrar(_signal=None, _frame=None):
        print("\nEncerrando assinante MQTT...")
        client.disconnect()
        sys.exit(0)

    def ao_conectar(cliente, _userdata, _flags, codigo_retorno):
        if codigo_retorno == 0:
            print("Conectado ao MQTT.")
            print(f"Broker: {config['mqtt_broker']}:{config['mqtt_port']}")
            print(f"Assinando: {config['mqtt_topic']}")
            cliente.subscribe(config["mqtt_topic"])
        else:
            print(f"Falha ao conectar no MQTT. Codigo: {codigo_retorno}")

    def ao_receber_mensagem(_cliente, _userdata, mensagem):
        valor = mensagem.payload.decode("utf-8", errors="replace")
        print(f"{mensagem.topic} -> {valor}")

    signal.signal(signal.SIGINT, encerrar)
    signal.signal(signal.SIGTERM, encerrar)

    client.on_connect = ao_conectar
    client.on_message = ao_receber_mensagem

    client.connect(config["mqtt_broker"], config["mqtt_port"])
    client.loop_forever()


if __name__ == "__main__":
    main()
