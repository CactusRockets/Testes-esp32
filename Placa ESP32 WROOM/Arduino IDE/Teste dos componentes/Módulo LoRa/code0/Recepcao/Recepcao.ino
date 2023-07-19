#include <HardwareSerial.h>

#define RX_PIN 16
#define TX_PIN 17

// Usando a Serial2 do ESP32
HardwareSerial LoRaSerial(2);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRaSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("Receiver setup");
}

void loop() {
  if (LoRaSerial.available()) {
    String message = LoRaSerial.readStringUntil('\n');

    Serial.print("Received message: ");
    Serial.println(message);
  }
}
