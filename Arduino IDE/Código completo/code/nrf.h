/* CONFIGURAÇÕES NRF24L01 */

#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include "nRF24L01.h"

#define SCK_PIN 18
#define MISO_PIN 19
#define MOSI_PIN 23

#define CE_PIN 12
#define CS_NRFPIN 2

// O tamanho deste pacote não deve exceder 32 bytes
struct PacketData {
  float tempo;
  float altitude;
  float velocidade;
  int parachute; 
  
  // 4+4+4+4=16bytes  
};

RF24 radio(CE_PIN, CS_NRFPIN);

// ANALISADO
uint8_t address[][6] = {"1Node", "2Node"};
bool radioNumber = 0;

// Usado para controlar funcionalidade do módulo rádio
// True = Tx role, False = RX role
bool role = true;

// ANALISADO
PacketData payload;
bool payloadReceived = false;

/* FUNÇÕES NRF */

void setupNRF() {
  delay(100);
  while(!radio.begin()) {
    Serial.println(F("Radio hardware is not responding!!"));
    delay(100);
  }
  delay(100);

  // ANALISADO
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);

  // TESTAR
  /*
  radio.setPALevel(RF24_PA_HIGH);
  radio.setPayloadSize(sizeof(payload));
  */

  // ANALISADO
  radio.openWritingPipe(address[radioNumber]);     
  radio.openReadingPipe(1, address[!radioNumber]); 

  if(role) {
    radio.stopListening();  
  } else {
    radio.startListening(); 
  }
}

void changeRoleNRF(bool functionNRF) {
  role = functionNRF;
  if(role) {
    radio.stopListening();  
  } else {
    radio.startListening(); 
  }
}

// ANALISAR
void updateNRF() {
  if(role) {
    // Funcionalidade do transmissor

    unsigned long start_timer = micros();                 
    bool report = radio.write(&payload, sizeof(PacketData));
    unsigned long end_timer = micros();                      

    if(report) {
      Serial.print(F("Transmission successful! "));
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);
      Serial.println(F("us"));
    } else {
      Serial.println(F("Transmission failed or timed out"));
    }

  } else {
    // Funcionalidade do receptor
    
    uint8_t pipe;
    if (radio.available(&pipe)) {
      
      uint8_t bytes = radio.getPayloadSize();
      radio.read(&payloadReceived, sizeof(bool));
      
      Serial.print(F("Received "));
      Serial.print(bytes);
      Serial.print(F(" bytes on pipe "));
      Serial.println(pipe);
      
      Serial.print(F("Pacote: "));
      Serial.println(payloadReceived);
    }
  }
}
