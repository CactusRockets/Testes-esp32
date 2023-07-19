/* CONFIGURAÇÕES LoRa */
#include "printf.h"

#include <HardwareSerial.h>

#define RX_PIN 16
#define TX_PIN 17

// Usando a Serial do ESP32
HardwareSerial LoRaSerial(2);
int contador = 0;



int size_data = sizeof(data);

void setupNRF() {
  //Configuração inicial do LoRa
  LoRaSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  while (!LoRaSerial);
}

void transmit() {
  LoRaSerial.write((const char*)&data, size_data);
}

void receive() {
  LoRaSerial.readBytes((const char*)aux, sizeof(bool));
}

void receive() {
  LoRaSerial.readBytes((char*)&receivedData, sizeof(SoloData));
}
