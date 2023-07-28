/* CONFIGURAÇÕES LORA */

#include <HardwareSerial.h>

// Pinos da Serial 2 do ESP32
#define RX2_PIN 16
#define TX2_PIN 17

// Usando a Serial 2 do ESP32
HardwareSerial LoRaSerial(2);
int sizeAllData = sizeof(allData);
int sizeSoloData = sizeof(soloData);



void setupTelemetry() {
  //Configuração inicial do LoRa
  LoRaSerial.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN);
  while(!LoRaSerial);

  Serial.println("LoRa conectado");
}

void transmit(const SoloData* soloData) {
  LoRaSerial.write((const char*)soloData, sizeSoloData);
  Serial.println();
}

void receive(AllPacketData *allData) {
  LoRaSerial.readBytes((char*)allData, sizeAllData);
}

void transmitString(String string) {
  LoRaSerial.println(string);
}

void receiveString() {
  message = LoRaSerial.readStringUntil('\n');
}
