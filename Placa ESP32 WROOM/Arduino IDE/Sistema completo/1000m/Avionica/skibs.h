/* CONFIGURAÇÕES SKIBS */

#define SKIB1 13
#define SKIB2 12
#define rangeMaximumAltitudeValue 2
#define rangeMinimumVelocityValue -5

// Em millisegundos
#define DELAY_FOR_STAGE2 5000

bool isDropping = false;

bool parachute1Activated = false;
bool parachute2Activated = false;

double timeForStage1 = 0;

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

  timeForStage1 = millis();
  parachute1Activated = true;
}

void activateStage2() {
  Serial.println("2º skib ativado!");
  digitalWrite(SKIB2, HIGH);

  parachute2Activated = true;
}
