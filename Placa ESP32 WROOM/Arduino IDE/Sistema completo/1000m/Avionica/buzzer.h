/* CONFIGURAÇÕES BUZZER */

#define buzzerPIN 14
#define tempoDeAtivacao 3000

void setupBuzzer() {
  pinMode(buzzerPIN, OUTPUT);
  Serial.println("Buzzer configurado!");
}

void activateBuzzer() {
  digitalWrite(buzzerPIN, HIGH);
  // Serial.println("Buzzer ativado!");
}

void desactivateBuzzer() {
  digitalWrite(buzzerPIN, LOW);
  // Serial.println("Buzzer desativado!");
}
