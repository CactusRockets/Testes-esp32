O código tem o objetivo de testar o funcionamento dos seguintes dispositivos:
- BMP280
- MPU6050
- LoRa

Configurações:
- Bibliotecas usadas:
  - heltec.h (https://github.com/HelTecAutomation/Heltec_ESP32)
  - Wire.h (utilizada para comunicação I2C)
  - Adafruit_BMP280.h (uso do BMP280)
  - Adafruit_MPU6050.h (uso do MPU6050)
  - Adafruit_Sensor.h (uso geral de qualquer sensor Adafruit)

## No Arduino IDE
Versão da IDE: 1.8.19
### Selecionando Placa Heltec WiFi LoRa 32 (V2)
#### Pinagem
Módulo SD:
- MOSI: GPIO27
- MISO: GPIO19
- SCLK: GPIO5
- CS: GPIO17
- VCC: 5V (do ESP32)
- GND: GND (do ESP32)
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
Nessa configuração o BMP e o MPU funcionam bem. O LoRa também funcionou apropriadamente (conseguiu enviar os pacotes), mas nada foi mostrado no display OLED.

OBS.: Apesar do módulo SD estar conectado no circuito, ele não é inicializado.

#### Pinagem
Módulo SD:
- MOSI: GPIO23
- MISO: GPIO19
- SCLK: GPIO18
- CS: GPIO17
- VCC: 5V (do ESP32)
- GND: GND (do ESP32)
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
Nessa configuração o BMP e o MPU funcionam bem. O LoRa também funcionou apropriadamente (conseguiu enviar os pacotes), mas nada foi mostrado no display OLED.

OBS.: Apesar do módulo SD estar conectado no circuito, ele não é inicializado.

### Selecionando Placa DOIT ESP32 Devkit V1
#### Pinagem
Módulo SD:
- MOSI: GPIO23
- MISO: GPIO19
- SCLK: GPIO18
- CS: GPIO17
- VCC: 5V (do ESP32)
- GND: GND (do ESP32)
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
Esse código não compila nessa configuração. Parece haver um certo conflito com a biblioteca heltec.h e a placa escolhida (DOIT ESP32 Devkit V1).

OBS.: Apesar do módulo SD estar conectado no circuito, ele não é inicializado.