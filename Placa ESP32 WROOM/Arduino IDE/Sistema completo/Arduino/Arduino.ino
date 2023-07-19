#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

#define CE_PIN 7
#define CS_PIN 8

#define BUTTON_PIN 2
int valueButton;

const uint64_t pipeIn =  0xE8E8F0F0E1LL;
const byte address[6] =  "00001";

bool activateParachute = false;
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
  radio.openWritingPipe(address);
  radio.openReadingPipe(1,pipeIn);
  radio.startListening();
}

void transmit() {
  if(radio.available()) {

    radio.stopListening();
    bool success = radio.write(&activateParachute, sizeof(bool));
    if(success) {
      Serial.print("Pacote enviado! ");
      Serial.print("Pacote: ");
      Serial.println(activateParachute);
    }
  }
}

void receive() {
  radio.startListening();
  while(radio.available()) {        
    radio.read(&data, sizeof(PacketData));

    // Armazena os dados em uma string
    String dados = String(data.time, 3)            //
      + "," + String(data.temperature, 3)          //
      + "," + String(data.altitude, 3)             //
      + "," + String(data.variation_altitude, 3)   //
      + "," + String(data.acceleration_Z, 3)       //
      + "," + String(data.altitude_MPU, 3)         //
      + "," + String(data.pressure, 3)             //
      + "," + String(data.parachute);              //
    Serial.println(dados);
  }
}


void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  setupNRF();
  Serial.println("NRF24L01 ativado!");
}

void loop() {
  receive();

  valueButton = digitalRead(BUTTON_PIN);
  if(valueButton == LOW) {
    activateParachute = true;
    transmit();
  }
}







