#include <SoftwareSerial.h>

#define RX_PIN 7
#define TX_PIN 8

SoftwareSerial mySerial(RX_PIN, TX_PIN);

void setup() {
  // Inicializa a comunicação serial com a velocidade de 9600 bps
  Serial.begin(9600);

  // Inicializa a SoftwareSerial com a mesma velocidade de 9600 bps
  mySerial.begin(9600);
}

void loop() {
  // Verifica se há dados disponíveis na SoftwareSerial
  if (mySerial.available()) {
    // byte disponível na SoftwareSerial
    char data = mySerial.read();

    // Faça algo com o dado lido, como exibi-lo na porta serial padrão
    Serial.print(data);
  }
}
