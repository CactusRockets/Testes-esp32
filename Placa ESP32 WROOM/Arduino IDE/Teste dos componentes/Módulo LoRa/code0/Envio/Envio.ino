#include <HardwareSerial.h>

#define RX_PIN 16
#define TX_PIN 17
#define M0 33
#define M1 32

// Usando a Serial2 do ESP32
HardwareSerial LoRaSerial(2);
int contador = 0;

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
  Serial.println("Transmitter setup");

  modoTransmissor();
}

void loop() {
  String message = "Hello, LoRa!" + String(" ") + String(contador);

  Serial.print("Sending message: ");
  Serial.println(message);
  LoRaSerial.println(message);

  contador++;
  // Aguarda entre cada transmissão
  // Deve ser um valor múltiplo de 500 milisegundos
  delay(500);
}
