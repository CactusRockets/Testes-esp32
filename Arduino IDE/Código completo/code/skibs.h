/* CONFIGURAÇÕES PARACHUTE */

#define SKIB1 16
#define SKIB2 17
#define rangeMaximumAltitudeValue 5
#define rangeMinimumVelocityValue -5

bool isDropping = false;

void setupSkibPins() {
  pinMode(SKIB1, OUTPUT);
  pinMode(SKIB2, OUTPUT);
}

void analyzeStateOfRocket() {
  if((maximumAltitudeValue - altitudeAtual) > rangeMaximumAltitudeValue) {

    if(velocidadeAtual < rangeMinimumVelocityValue) {
      isDropping = true;
    }
  }
}

void activateSkibs() {
  Serial.println("Primeiro skib ativado!");
  digitalWrite(SKIB1, HIGH);
  Serial.println("Segundo skib ativado!");
  digitalWrite(SKIB2, HIGH);
}
