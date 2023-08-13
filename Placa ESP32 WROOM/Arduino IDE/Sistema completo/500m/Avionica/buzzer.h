/* CONFIGURAÇÕES BUZZER */

#define buzzerPIN 14
#define tempoDeAtivacao 3000

void setupBuzzer() {
  pinMode(buzzerPIN, OUTPUT);
  Serial.println("Buzzer configurado!");
}

void activateBuzzer() {
  digitalWrite(buzzerPIN, HIGH);
  Serial.println("Buzzer ativado!");
}
