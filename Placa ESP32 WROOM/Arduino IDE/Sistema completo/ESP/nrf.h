/* CONFIGURAÇÕES NRF24L01 */
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include "nRF24L01.h"

#include <HardwareSerial.h>

#define RX_PIN 16
#define TX_PIN 17

// Usando a Serial2 do ESP32
HardwareSerial LoRaSerial(2);
int contador = 0;

void setupNRF() {
  //Configuração inicial do LoRa
  LoRaSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  while (!LoRaSerial);

  // Outras configurações
}

void transmit() {
  Serial.print("Sending message: ");
  Serial.println(message);
  LoRaSerial.println(message);
}

void receive() {
  radio.startListening();
  if(radio.available()) {        
    radio.read(&aux, sizeof(aux));

    Serial.print("Valor do Paraquedas:");
    Serial.println(aux);
  }
}
