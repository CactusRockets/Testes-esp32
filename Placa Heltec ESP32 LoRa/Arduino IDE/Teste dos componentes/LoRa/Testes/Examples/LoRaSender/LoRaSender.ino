#include "heltec.h"
#define BAND 433E6

int counter = 0;

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
  Serial.print("Sending packet: ");
  Serial.println(counter);
  // Send packet
  LoRa.beginPacket();
  
  /*
   * LoRa.setTxPower(txPower, RFOUT_pin);
   * txPower -- 0 ~ 20
   * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
   *   - RF_PACONFIG_PASELECT_PABOOST
   *        LoRa single output via PABOOST, maximum output 20dBm
   *   - RF_PACONFIG_PASELECT_RFO
   *        LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
  */
  LoRa.setTxPower(14, RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();
  
  counter++;

  // Turn the LED on (HIGH is the voltage level)
  digitalWrite(25, HIGH);
  delay(1000);
  // Turn the LED off by making the voltage LOW
  digitalWrite(25, LOW);    
  delay(1000);
}
