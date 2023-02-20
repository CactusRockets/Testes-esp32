#include "heltec.h"

#define BAND 433E6
void setup() {
  // WIFI Kit series V1 not support Vext control

  /* DisplayEnable Enable */
  /* Heltec.LoRa Disable */
  /* Serial Enable */
  /* PABOOST Enable */
  /* long BAND */
  Heltec.begin(true, true, true, true, BAND);
}

void loop() {
  Serial.println("Analisando pacotes disponíveis!");
  
  // Try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Received a packet
    Serial.print("Received packet '");
    // Read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    // Print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
