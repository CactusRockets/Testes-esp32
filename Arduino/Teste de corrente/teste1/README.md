## No Arduino IDE
Versão da IDE: 1.8.19
### Selecionando Placa Heltec WiFi LoRa 32 (V2)
#### Pinagem
BMP280:
- SDA: 21
- SCL: 22
- VCC: 3.3V (do ESP32)
- GND: GND (do ESP32)
MPU6050:
- SDA: 21
- SCL: 22
- VCC: 3.3V (do ESP32)
- GND: GND (do ESP32)

#### Detalhes
O LoRa nesse código estava demorando muito para enviar os pacotes. Estava demorando cerca de 1.2 segundos para enviar um pacote. Suspeito que o problema esteja relacionado com a comunicação I2C. Pois, quando foi testado somente o LoRa (no código de teste de distância, por exemplo), o envio dos pacotes aconteceu rapidamente.

Nessa configuração o BMP e o MPU funcionam bem.
