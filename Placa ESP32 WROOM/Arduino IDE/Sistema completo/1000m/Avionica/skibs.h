/* CONFIGURAÇÕES SKIBS */

#define SKIB1 13
#define SKIB2 12
#define rangeMaximumAltitudeValue 20
#define rangeMinimumVelocityValue -5

// Em ms
#define TIME_BETWEEN_ACTIVATIONS 8000

// Em metros
#define GAP_BETWEEN_ACTIVATIONS 150

// Em metros
#define HEIGHT_FOR_2_STAGE 500

// Em millisegundos
#define SKIB_TIME 250

bool isDropping = false;

bool parachute1Activated = false;
bool parachute2Activated = false;

double timeForStage1 = 0;
double timeForStage2 = 0;

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
  digitalWrite(SKIB1, HIGH);
  Serial.println("1 Skib ativado!");

  timeForStage1 = millis();
  parachute1Activated = true;
}

void activateStage2() {
  digitalWrite(SKIB2, HIGH);
  Serial.println("2 Skib ativado!");

  timeForStage2 = millis();
  parachute2Activated = true;
}

void deactivateStage1() {
  digitalWrite(SKIB1, LOW);
  Serial.println("1 Skib desativado!");
}

void deactivateStage2() {
  digitalWrite(SKIB2, LOW);
  Serial.println("2 Skib desativado!");
}
