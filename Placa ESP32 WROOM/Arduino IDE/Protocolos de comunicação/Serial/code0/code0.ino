#include <HardwareSerial.h>

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
}

void loop() {
  Serial.println("This message is sent to USB Serial");
  Serial1.println("This message is sent to UART1 Serial");
  Serial2.println("This message is sent to UART2 Serial");

  delay(1000);
}