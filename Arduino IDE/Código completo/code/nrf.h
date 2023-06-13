/* CONFIGURAÇÕES NRF24L01 */

#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include "nRF24L01.h"

#define CE_PIN 12
#define CS_NRFPIN 2

const uint64_t pipeOut =  0xE8E8F0F0E1LL;

RF24 radio(CE_PIN, CS_NRFPIN);

void setupNRF() {
  //Configuração do NRF24L01+
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
}

void transmit() {
  radio.write(&data, sizeof(PacketData));
}
