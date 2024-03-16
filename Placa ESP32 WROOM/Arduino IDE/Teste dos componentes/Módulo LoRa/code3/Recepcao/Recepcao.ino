/*
Os pinos M0 e M1 do módulo LoRa foram conectados diretamente no GND
*/

#define LED1 4  
 
void setup() {
  pinMode(LED1, OUTPUT);
  Serial.begin(9600);
}
 
void loop() { 
  if(Serial.available() > 1){
    String input = Serial.readString();

    if(input == "on") {
      digitalWrite(LED1, HIGH);  
    } 
    if(input == "off") {
      digitalWrite(LED1, LOW);
    }
  }
  delay(20);
}