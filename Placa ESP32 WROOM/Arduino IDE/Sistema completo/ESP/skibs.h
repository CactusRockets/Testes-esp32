/* CONFIGURAÇÕES SKIBS */

#define SKIB1 13
#define SKIB2 12
#define rangeMaximumAltitudeValue 2
#define rangeMinimumVelocityValue -5

bool isDropping = false;
bool parachuteActivated = false;

void setupSkibPins() {
  pinMode(SKIB1, OUTPUT);
  pinMode(SKIB2, OUTPUT);
}

void analyzeStateOfRocket() {
  if((maximumAltitudeValue - altitudeAtual) > rangeMaximumAltitudeValue) {
    isDropping = true;
  }
  if(soloData.parachute == 1) {
    isDropping = true;
  }
}

void activateSkibs() {
  Serial.println("Primeiro skib ativado!");
  digitalWrite(SKIB1, HIGH);
  Serial.println("Segundo skib ativado!");
  digitalWrite(SKIB2, HIGH);

  parachuteActivated = true;
}
