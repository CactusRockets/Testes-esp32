/* CONFIGURAÇÕES SKIBS */

#define SKIB1 13
#define SKIB2 12
#define rangeMaximumAltitudeValue 2
#define rangeMinimumVelocityValue -5

// Metros de diferença para o o acionamento do segundo estágio
#define METERS_DIFFERENCE 500

bool isDropping = false;

bool parachute1Activated = false;
bool parachute2Activated = false;

double altitudeForStage1 = 0;

void setupSkibPins() {
  pinMode(SKIB1, OUTPUT);
  pinMode(SKIB2, OUTPUT);
  Serial.println("Skibs configurados!");
}

void analyzeStateOfRocket() {
  if((maximumAltitudeValue - altitudeAtual) > rangeMaximumAltitudeValue) {
    isDropping = true;
  }
  if(soloData.parachute == 1) {
    isDropping = true;
  }
}

void activateStage1() {
  Serial.println("1º skib ativado!");
  digitalWrite(SKIB1, HIGH);

  altitudeForStage1 = altitudeAtual;
  parachute1Activated = true;
}

void activateStage2() {
  Serial.println("2º skib ativado!");
  digitalWrite(SKIB2, HIGH);

  parachute2Activated = true;
}
