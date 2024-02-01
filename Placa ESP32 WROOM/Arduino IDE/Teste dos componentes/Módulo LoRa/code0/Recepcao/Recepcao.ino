#include <HardwareSerial.h>

#define RX_PIN 16
#define TX_PIN 17
#define M0 33
#define M1 32

HardwareSerial LoRaSerial(2);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  LoRaSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("Receiver setup");
}

void loop() {
  if (LoRaSerial.available() > 1) {
    String message = LoRaSerial.readString();

    Serial.print("Received message: ");
    Serial.println(message);
  }
}
