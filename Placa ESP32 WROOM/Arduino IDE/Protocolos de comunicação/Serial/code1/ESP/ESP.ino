#include <HardwareSerial.h>

#define RX2_PIN 16
#define TX2_PIN 17

#define RX1_PIN 15
#define TX1_PIN 4

HardwareSerial mySerial2(2);
HardwareSerial mySerial1(1);
String message;

void setup() {
  message = "Daniel Alencar Penha Carvalho";

  // Inicializa a comunicação serial com a velocidade de 9600 bps
  Serial.begin(9600);

  // Inicializa a HardwareSerial com a mesma velocidade de 9600 bps
  mySerial2.begin(9600, SERIAL_8N1, RX2_PIN, TX2_PIN);
  while(!mySerial2);

  // Inicializa a HardwareSerial com a mesma velocidade de 9600 bps
  mySerial1.begin(9600, SERIAL_8N1, RX1_PIN, TX1_PIN);
  while(!mySerial1);
}

void loop() {
  // Envio dos dados
  Serial.println(message);
  mySerial2.println(message);
  mySerial1.println(message);
  delay(200);
}
