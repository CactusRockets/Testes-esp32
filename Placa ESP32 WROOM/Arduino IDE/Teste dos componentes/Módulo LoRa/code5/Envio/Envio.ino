/*
Os pinos M0 e M1 do módulo LoRa foram conectados diretamente no GND
*/

#define BTN1  4
#define BTN2  5

String turnOn = "00000,00000,00000,00000,00000,00000,00000,00000,00000,0000";
String turnOff = "10000,00000,00000,00000,00000,00000,00000,00000,00000,0000";

HardwareSerial loraSerial(2);

void setup() {
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  Serial.begin(9600);
  loraSerial.begin(9600, SERIAL_8N1, 16, 17);
}
 
void loop() {
  loraSerial.print(turnOn);
  Serial.println(turnOn);
  delay(1250);

  loraSerial.print(turnOff);
  Serial.println(turnOff);
  delay(1250);
}