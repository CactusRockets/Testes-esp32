/*
Os pinos M0 e M1 do módulo LoRa foram conectados diretamente no GND
*/

#define LED1 4
#define LED2 2

HardwareSerial LoRaSerial(2);
 
void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2,OUTPUT);
  Serial.begin(9600);

  LoRaSerial.begin(9600, SERIAL_8N1, 16, 17);
}

void loop() {
  
  if(LoRaSerial.available() > 1){
    String input = LoRaSerial.readString();
    Serial.println(input);

    if(input == "00000,00000,00000,00000,00000,00000,00000,00000,00000,0000") {
      digitalWrite(LED2, HIGH);  
    } 
    if(input == "10000,00000,00000,00000,00000,00000,00000,00000,00000,0000") {
      digitalWrite(LED2, LOW);
    }
  }
  delay(20);
}