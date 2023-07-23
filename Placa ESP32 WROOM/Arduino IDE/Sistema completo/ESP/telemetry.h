/* CONFIGURAÇÕES LORA */

#include "printf.h"

#include <HardwareSerial.h>

// Pinos da Serial 2 do ESP32
#define RX2_PIN 16
#define TX2_PIN 17

// Usando a Serial 2 do ESP32
HardwareSerial LoRaSerial(2);



void setupTelemetry() {
  //Configuração inicial do LoRa
  LoRaSerial.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN);
  while(!LoRaSerial);
}

void transmit() {
  LoRaSerial.write((const char*)&allData, sizeof(AllPacketData));
}

void receive() {
  LoRaSerial.readBytes((char*)&receivedData, sizeof(SoloData));
}