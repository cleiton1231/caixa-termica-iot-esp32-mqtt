import os

from dotenv import load_dotenv


REQUIRED_KEYS = [
    "MYSQL_HOST",
    "MYSQL_PORT",
    "MYSQL_DATABASE",
    "MYSQL_USER",
    "MYSQL_PASSWORD",
    "MQTT_BROKER",
    "MQTT_PORT",
    "MQTT_CLIENT_ID",
]


def mask_value(key, value):
    if value is None or value == "":
        return "NAO_CONFIGURADO"
    if "PASSWORD" in key or "SENHA" in key:
        return "***"
    return value


def main():
    load_dotenv()

    missing = []
    print("Verificacao do .env")
    print("-------------------")

    for key in REQUIRED_KEYS:
        value = os.getenv(key)
        if not value:
            missing.append(key)
        print(f"{key}={mask_value(key, value)}")

    if missing:
        print("\nVariaveis faltando:")
        for key in missing:
            print(f"- {key}")
        raise SystemExit(1)

    print("\n.env carregado com sucesso.")


if __name__ == "__main__":
    main()
