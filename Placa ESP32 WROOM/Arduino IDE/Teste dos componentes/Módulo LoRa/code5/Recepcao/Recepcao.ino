/*
Os pinos M0 e M1 do módulo LoRa foram conectados diretamente no GND
*/

#define LED1 4

HardwareSerial LoRaSerial(2);
 
void setup() {
  pinMode(LED1, OUTPUT);
  Serial.begin(9600);

  LoRaSerial.begin(9600, SERIAL_8N1, 16, 17);
}
 
void loop() {
  if(LoRaSerial.available() > 1){
    String input = LoRaSerial.readString();
    Serial.println(input);
    LoRaSerial.print(input);

    if(input == "on") {
      digitalWrite(LED1, HIGH);  
    } 
    if(input == "off") {
      digitalWrite(LED1, LOW);
    }
  }
  delay(500);
}