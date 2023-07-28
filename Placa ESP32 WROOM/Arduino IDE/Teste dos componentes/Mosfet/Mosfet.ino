// Definir o pino que será controlado
#define pinoLED1 13
#define pinoLED2 12

void setup() {
  Serial.begin(115200);

  // Configurar o pino como saída
  pinMode(pinoLED1, OUTPUT);
  pinMode(pinoLED2, OUTPUT);
}

void loop() {
  Serial.print("Millis: ");
  Serial.println(millis());

  if(millis() > 15000) {
    // Ativar o LED (pino 4)
    digitalWrite(pinoLED1, HIGH);
    digitalWrite(pinoLED2, HIGH);
    
    // Aguardar 1 segundo (1000 milissegundos)
    delay(1000);
    
    // Desativar o LED (pino 4)
    digitalWrite(pinoLED1, LOW);
    digitalWrite(pinoLED2, LOW);
    
    // Aguardar 1 segundo (1000 milissegundos)
    delay(1000);
  }
}
