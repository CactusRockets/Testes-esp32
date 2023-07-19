#include <HardwareSerial.h>

#define RX_PIN 16
#define TX_PIN 17

// Usando a Serial do ESP32
HardwareSerial LoRaSerial(2);
int contador = 0;



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

PacketData data = {
  1.0,
  1.0,
  1.0,
  1.0,
  1.0,
  1.0,
  1.0,
  1
};

int size_data = sizeof(data);



void send(const PacketData* data) {
  LoRaSerial.write((const char*)data, size_data);
}

void receive(PacketData* data) {
  LoRaSerial.readBytes((char*)data, sizeof(PacketData));
}



void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRaSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("Transmitter");
}

void loop() {
  send(&data);
  Serial.println();

  data.parachute++;

  delay(1000);
}
