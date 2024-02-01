#define RX_PIN 16
#define TX_PIN 17
#define M0 35
#define M1 34

HardwareSerial LoRaSerial(2);
int contador = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  LoRaSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("Transmitter setup");
}

void loop() {
  String message = "Hello, LoRa!" + String(" ") + String(contador);

  Serial.print("Sending message: ");
  Serial.println("message");
  LoRaSerial.print("message");

  contador++;
  // Aguarda entre cada transmissão
  // Deve ser um valor múltiplo de 500 milisegundos
  delay(500);
}
