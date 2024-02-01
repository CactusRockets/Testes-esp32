/*
Os pinos M0 e M1 do módulo LoRa foram conectados diretamente no GND
*/

#define BTN1 4
#define BTN2 5

#define M0 35
#define M1 34
 
String turnOn = "on";
String turnOff = "off";

HardwareSerial LoRaSerial(2);

void setup() {
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  Serial.begin(9600);

  pinMode(M0, INPUT);
  pinMode(M1, INPUT);

  LoRaSerial.begin(9600, SERIAL_8N1, 16, 17);
}
 
void loop() {
 
  Serial.println(turnOn);
  LoRaSerial.println(turnOn);
  delay(500);

  Serial.println(turnOff);
  LoRaSerial.println(turnOff);
  delay(500);
}