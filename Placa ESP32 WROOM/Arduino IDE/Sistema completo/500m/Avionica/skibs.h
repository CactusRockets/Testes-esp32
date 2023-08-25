/* CONFIGURAÇÕES SKIBS */

#define SKIB1 13
#define SKIB2 12
#define rangeMaximumAltitudeValue 2
#define rangeMinimumVelocityValue -5

#define DELAY_FOR_STAGE 250

bool isDropping = false;
bool parachuteActivated = false;

double timeForStage = 0;

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

void activateSkibs() {
  Serial.println("1º skib ativado!");
  digitalWrite(SKIB1, HIGH);
  delay(250);
  Serial.println("1º skib desativado!");
  digitalWrite(SKIB1, LOW);

  Serial.println("2º skib ativado!");
  digitalWrite(SKIB2, HIGH);
  delay(250);
  Serial.println("2º skib desativado!");
  digitalWrite(SKIB2, LOW);

  timeForStage = millis();
  parachuteActivated = true;
}

void deactivateSkibs() {
  // Serial.println("1º skib desativado!");
  digitalWrite(SKIB1, LOW);
  // Serial.println("2º skib desativado!");
  digitalWrite(SKIB2, LOW);
}
