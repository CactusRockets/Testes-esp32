/* CONFIGURAÇÕES NRF24L01 */

#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include "nRF24L01.h"

#define CE_PIN 12
#define CS_NRFPIN 2

const uint64_t pipeOut =  0xE8E8F0F0E1LL;
const byte address[6] =  "00001";

bool soloMessage = false;
RF24 radio(CE_PIN, CS_NRFPIN);

void setupNRF() {
  //Configuração do NRF24L01+
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);

  radio.openWritingPipe(pipeOut);
  radio.openReadingPipe(1,address);
}

void transmit() {
  radio.stopListening();
  radio.write(&data, sizeof(PacketData));
}

void receive() {
  radio.startListening();
  if(radio.available()) {        
    radio.read(&soloMessage, sizeof(soloMessage));

    Serial.print("Valor do Paraquedas:");
    Serial.println(soloMessage);
  }
}
