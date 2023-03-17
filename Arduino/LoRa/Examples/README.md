Os códigos desta pasta testam o funcionamento básico do LoRa embutido no próprio ESP32 da Heltec.
Nesta pasta temos os seguintes códigos:
- LoRaReceiver: Testa o dispositivo LoRa como receptor
- LoRaSender: Testa o dispositivo LoRa como transmissor
- OLED_LoRa_Receiver: Testa o LoRa como receptor e o OLED do dispositivo
- OLED_LoRa_Sender: Testa o LoRa como transmissor e o OLED do dispositivo

Esses exemplos foram testados na IDE do arduino com as seguintes configurações
- Placa: Heltec WiFi LoRa 32 (V2)
- Versão da IDE: 1.8.19
- Bibliotecas usadas:
  - heltec (https://github.com/HelTecAutomation/Heltec_ESP32)