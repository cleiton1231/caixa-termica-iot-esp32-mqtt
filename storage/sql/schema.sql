CREATE DATABASE IF NOT EXISTS iot_caixa_termica
    CHARACTER SET utf8mb4
    COLLATE utf8mb4_unicode_ci;

USE iot_caixa_termica;

CREATE TABLE IF NOT EXISTS leituras_caixa (
    id INT AUTO_INCREMENT PRIMARY KEY,
    topico VARCHAR(128) NOT NULL,
    valor VARCHAR(128) NOT NULL,
    criado_em TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    INDEX idx_leituras_topico (topico),
    INDEX idx_leituras_criado_em (criado_em)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
