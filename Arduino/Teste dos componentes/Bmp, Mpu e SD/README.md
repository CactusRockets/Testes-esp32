O código tem o objetivo de testar o funcionamento dos seguintes dispositivos:
- BMP280
- MPU6050
- MicroSD Reader

Configurações:
- Bibliotecas usadas:
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
Aparentemente, nessa configuração o BMP e o MPU funcionam bem. O único problema é o módulo de cartão SD que às vezes funciona e outras vezes não. O problema aparentemente é com a comunicação SPI do Esp32 e do módulo.

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
Nessa configuração o BMP, MPU e o módulo de cartão SD funcionam bem. Mas pode dar problema na pinagem quando utilizarmos o LoRa embutido no ESP32. 