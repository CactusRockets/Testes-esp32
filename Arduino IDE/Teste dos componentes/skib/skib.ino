#define SKIB1 12
#define SKIB2 13

#define ENABLE 1

void setup() {
  Serial.begin(115200);
  pinMode(SKIB1, OUTPUT);
  pinMode(SKIB2, OUTPUT);
}

void loop() {
  float executionTime = millis();
  Serial.println(executionTime);

  if(ENABLE) {
    if(executionTime >= 30000) {
      Serial.println("Segundo skib ativado!");
      digitalWrite(SKIB2, HIGH);
    } else if (executionTime >= 20000) {
      Serial.println("Primeiro skib ativado!");
      digitalWrite(SKIB1, HIGH);
    } else {
      Serial.println("Desativado");
    }
  } else {
    Serial.println("Desativado");
  }

  delay(500);
}
