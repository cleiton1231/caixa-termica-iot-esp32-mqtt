# Hardware

## Lista de componentes

| Componente | Funcao |
|---|---|
| ESP32 | Microcontrolador principal, Wi-Fi e execucao do firmware |
| DS18B20 a prova d'agua | Medicao de temperatura interna |
| LDR | Deteccao de luminosidade como indicio de abertura |
| MPU6050 | Medicao de aceleracao para impacto, vibracao ou inclinacao |
| LED | Sinalizacao visual local |
| Buzzer | Alerta sonoro local |
| Display OLED I2C | Saida visual local opcional |
| Resistor para LDR | Divisor de tensao para leitura analogica |
| Protoboard ou placa de montagem | Interligacao dos componentes |
| Jumpers | Conexoes eletricas |
| Fonte ou cabo USB | Alimentacao do ESP32 |

## Pinagem provisoria

| Sinal | GPIO ESP32 | Observacao |
|---|---:|---|
| DS18B20 DATA | 4 | Requer resistor de pull-up conforme montagem |
| LDR | 34 | Entrada analogica, apenas entrada |
| Buzzer | 25 | Saida digital |
| LED | 2 | Saida digital, geralmente LED onboard em algumas placas |
| I2C SDA | 21 | Barramento I2C para MPU6050 e OLED opcional |
| I2C SCL | 22 | Barramento I2C para MPU6050 e OLED opcional |

## Cuidados com ESP32 em 3,3 V

O ESP32 trabalha com logica de 3,3 V. Sensores, divisores de tensao e modulos devem ser ligados de forma compativel com esse nivel. Entradas do ESP32 nao devem receber 5 V diretamente.

## GPIO 34

O GPIO 34 e adequado para leitura analogica do LDR por ser uma entrada ADC. Ele nao funciona como saida digital, portanto deve ser usado apenas para leitura.

## GPIOs a evitar

Evitar o uso dos GPIOs 6 a 11, pois normalmente sao ligados a memoria flash interna do modulo ESP32. O uso incorreto desses pinos pode impedir o funcionamento da placa.

## Pontos a validar na montagem fisica

- Confirmar resistor de pull-up do DS18B20.
- Confirmar divisor de tensao do LDR com valor adequado.
- Validar se o buzzer usado pode ser acionado diretamente pelo GPIO ou se exige transistor.
- Confirmar endereco I2C do MPU6050 e do OLED, caso usado.
- Validar limiar real de luminosidade com a caixa fechada e aberta.
- Validar limiar real de aceleracao com movimentos controlados.
- Confirmar estabilidade da alimentacao durante os testes.
