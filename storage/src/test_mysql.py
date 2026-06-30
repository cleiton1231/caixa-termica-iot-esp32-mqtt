import os

import mysql.connector
from dotenv import load_dotenv


def carregar_configuracao():
    load_dotenv()
    return {
        "host": os.getenv("MYSQL_HOST", "localhost"),
        "port": int(os.getenv("MYSQL_PORT", "3306")),
        "database": os.getenv("MYSQL_DATABASE", "iot_caixa_termica"),
        "user": os.getenv("MYSQL_USER", "root"),
        "password": os.getenv("MYSQL_PASSWORD", ""),
    }


def conectar(config):
    return mysql.connector.connect(
        host=config["host"],
        port=config["port"],
        database=config["database"],
        user=config["user"],
        password=config["password"],
    )


def main():
    config = carregar_configuracao()
    print(f"Conectando ao MySQL em {config['host']}:{config['port']}...")

    conexao = conectar(config)
    cursor = conexao.cursor()

    try:
        cursor.execute("SHOW TABLES LIKE 'leituras_caixa'")
        tabela = cursor.fetchone()
        if tabela is None:
            raise SystemExit("Tabela leituras_caixa nao encontrada. Execute storage/sql/schema.sql primeiro.")

        cursor.execute(
            "INSERT INTO leituras_caixa (topico, valor) VALUES (%s, %s)",
            ("teste/mysql", "OK"),
        )
        conexao.commit()
        novo_id = cursor.lastrowid

        cursor.execute(
            "SELECT id, topico, valor, criado_em FROM leituras_caixa WHERE id = %s",
            (novo_id,),
        )
        linha = cursor.fetchone()
        print("Insercao de teste realizada com sucesso:")
        print(linha)
    finally:
        cursor.close()
        conexao.close()


if __name__ == "__main__":
    main()
