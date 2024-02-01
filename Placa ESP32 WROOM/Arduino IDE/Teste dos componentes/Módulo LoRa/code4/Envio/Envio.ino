/*
Os pinos M0 e M1 do módulo LoRa foram conectados diretamente no GND
*/

#define BTN1  4
#define BTN2  5  
 
String turnOn = "on";
String turnOff = "off";

void setup() {
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  Serial.begin(9600);
}
 
void loop() {
 
  if(digitalRead(BTN1) == 0) {
    Serial.print(turnOn);
    while(digitalRead(BTN1) == 0);
    delay(50);
  }
 
  if(digitalRead(BTN2) == 0) {
    Serial.print(turnOff);
    while(digitalRead(BTN2) == 0);
    delay(50);
  }
}