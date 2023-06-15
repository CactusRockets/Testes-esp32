#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

#define CE_PIN 7
#define CS_PIN 8

const uint64_t pipeIn =  0xE8E8F0F0E1LL;
RF24 radio(CE_PIN, CS_PIN);


// O tamanho deste pacote não deve exceder 32 bytes
struct PacketData {
  float time;
  float altitude_MPU;
  float temperature;
  float pressure;
  float altitude;
  float variation_altitude;
  float acceleration_Z;
  int parachute; 
};
PacketData data;

void setupNRF() {
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(false);
  radio.openReadingPipe(1,pipeIn);
  radio.startListening();
}

void receive() {
  while(radio.available()) {        
    radio.read(&data, sizeof(PacketData));

    // Armazena os dados em uma string
    String dados = String(data.time, 3)            //
      + "," + String(data.temperature, 3)          //
      + "," + String(data.altitude, 3)             //
      + "," + String(data.variation_altitude, 3)   //
      + "," + String(data.acceleration_Z, 3)       //
      + "," + String(data.altitude_MPU, 3)
      + "," + String(data.pressure, 3)             //
      + "," + String(data.parachute);              //
    Serial.println(dados);
  }
}


void setup() {
  Serial.begin(115200);

  setupNRF();
  Serial.println("NRF24L01 ativado!");
}

void loop() {
  receive();
}







