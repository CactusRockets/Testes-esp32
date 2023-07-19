#include <HardwareSerial.h>

#define RX_PIN 16
#define TX_PIN 17

// Usando a Serial2 do ESP32
HardwareSerial LoRaSerial(2);
int contador = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  LoRaSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("Transmitter setup");
}

void loop() {
  String message = "Hello, LoRa!" + String(" ") + String(contador);

  Serial.print("Sending message: ");
  Serial.println(message);
  LoRaSerial.println(message);

  contador++;
  // Aguarda entre cada transmissão
  delay(2000);
}
