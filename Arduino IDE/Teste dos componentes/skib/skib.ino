#define PIN 12
#define ENABLE 1

void setup() {
  Serial.begin(115200);
  pinMode(PIN, OUTPUT);
}

void loop() {
  float executionTime = millis();
  
  if(executionTime >= 15000 && ENABLE) {
    Serial.println("Ativado");
    digitalWrite(PIN, HIGH);
  } else {
    Serial.println("Desativado");
    digitalWrite(PIN, LOW);
  }

  delay(500);
}
