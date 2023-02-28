O código tem o objetivo de testar o funcionamento dos seguintes dispositivos:
- BMP280
- MPU6050
- MicroSD Reader
- LoRa
E também testar o acionamento de algum pino do ESP32

Configurações:
- Bibliotecas usadas:
  - heltec.h (https://github.com/HelTecAutomation/Heltec_ESP32)
  - Wire.h (utilizada para comunicação I2C)
  - SD.h (utilizada para o uso do módulo de cartão SD)
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
O código compila e inicializou corretamente todos os dispositivos todos os dispositivos: BMP, MPU, SD module e LoRa. O acionamento do pino 12 também ocorreu perfeitamente.
