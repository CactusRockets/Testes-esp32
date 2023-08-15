#include <HardwareSerial.h>

#define RX_PIN 16
#define TX_PIN 17
#define M0 33
#define M1 32

// Usando a Serial2 do ESP32
HardwareSerial LoRaSerial(2);

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

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  LoRaSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("Receiver setup");

  modoReceptor();
}

void loop() {
  if (LoRaSerial.available()) {
    String message = LoRaSerial.readStringUntil('\n');

    Serial.print("Received message: ");
    Serial.println(message);
  }
}
