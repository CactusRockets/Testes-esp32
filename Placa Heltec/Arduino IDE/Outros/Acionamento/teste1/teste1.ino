#define PIN1 16
#define PIN2 17

void setup() {
  pinMode(PIN1, OUTPUT); 
  pinMode(PIN2, OUTPUT);
}

void loop() {
  digitalWrite(PIN1, HIGH);
  digitalWrite(PIN2, HIGH);
  delay(1000);
  digitalWrite(PIN1, LOW);
  digitalWrite(PIN2, LOW);
  delay(1000);
}
