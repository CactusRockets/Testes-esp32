
#define buzzerPIN 12
#define tempoDeAtivacao 3000

void setupBuzzer() {
  pinMode(buzzerPIN, OUTPUT);
}

void activateBuzzer() {
  digitalWrite(buzzerPIN, HIGH);
}
