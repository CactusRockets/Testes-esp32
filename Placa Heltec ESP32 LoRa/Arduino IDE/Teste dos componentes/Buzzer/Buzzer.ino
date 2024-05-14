#define buzzerAtivo 4
#define tempoDeAtivacao 3000

void bipAtivo(int tempoBip) {
  digitalWrite(buzzerAtivo, HIGH);
  delay(tempoBip);
  digitalWrite(buzzerAtivo, LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(buzzerAtivo, OUTPUT);
}

void loop() {
  if(millis() > 5000) {
    bipAtivo(tempoDeAtivacao);
    Serial.println("Ativado");
  } else {
    Serial.println("Desativado");
  }
  
  delay(500);
}
