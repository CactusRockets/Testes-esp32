#include <HardwareSerial.h>

#define RX_PIN 16
#define TX_PIN 17

// Usando a Serial do ESP32
HardwareSerial LoRaSerial(2);




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
  Serial.println("Receiver");
}

void loop() {
  if(LoRaSerial.available()) {
    receive(&data);

    // Armazena os dados em uma string
    String dados = String(data.time, 3)         
      + "," + String(data.temperature, 3)       
      + "," + String(data.altitude, 3)          
      + "," + String(data.variation_altitude, 3)
      + "," + String(data.acceleration_Z, 3)    
      + "," + String(data.altitude_MPU, 3)      
      + "," + String(data.pressure, 3)          
      + "," + String(data.parachute);           
    Serial.println(dados);
  }
}
