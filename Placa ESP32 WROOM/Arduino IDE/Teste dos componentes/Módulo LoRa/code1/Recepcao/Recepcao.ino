#include <HardwareSerial.h>

#define RX_PIN 16
#define TX_PIN 17
#define M0 33
#define M1 32

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


void modoReceptor() {
  // Configurações para modo Receptor
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
}

void modoTransmissor() {
  // Configurações para modo Transmissor
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
}

void send(const PacketData* data) {
  modoTransmissor();
  LoRaSerial.write((const char*)data, size_data);
}

void receive(PacketData* data) {
  modoReceptor();
  LoRaSerial.readBytes((char*)data, sizeof(PacketData));
}

void printData() {
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


void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  LoRaSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("Receiver");
}

void loop() {
  if(LoRaSerial.available()) {
    receive(&data);
    printData();
  }
}
