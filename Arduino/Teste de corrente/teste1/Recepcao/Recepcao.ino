/*
  This is a simple example show the Heltec.LoRa recived data in OLED.

  The onboard OLED display is SSD1306 driver and I2C interface. In order to make the
  OLED correctly operation, you should output a high-low-high(1-0-1) signal by soft-
  ware to OLED's reset pin, the low-level signal at least 5ms.

  OLED pins to ESP32 GPIOs via this connecthin:
  OLED_SDA -- GPIO4
  OLED_SCL -- GPIO15
  OLED_RST -- GPIO16
*/

#include "heltec.h" 
#include "images.h"

/* PARÂMETROS */
#define BAND 433E6

// Pinos de comunicação LoRa
#define SS 18
#define RESET 14
#define DIO0 26

// Frequência de comunicação SPI
#define frequency_SPI 10E6
// Quanto maior o fator de espalhamento, menor será o valor da sensibilidade do rádio (para recepção)
// Quanto maior o fator de espalhamento, menor a taxa de transmissão dos dados
#define spreadingFactor 7
// Se refere a quantidade máxima de bits/s que determinada transmissão suporta
#define signalBandwidth 125E3
// O CRC é uma técnica usada para detectar erros em dados digitais
#define CRC 0

String rssi = "RSSI --";
String packSize = "--";
String packet;

void logo(){
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void LoRaData(){
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, rssi);  
  Heltec.display->drawString(0, 10, "Received "+ packSize + " bytes");
  Heltec.display->drawStringMaxWidth(0 , 20 , 128, packet);
  Heltec.display->display();
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  LoRaData();
}

void setup() {
  // WIFI Kit series V1 not support Vext control

  // Setup LoRa
  LoRa.setPins(SS, RESET, DIO0);
  LoRa.setSPIFrequency(frequency_SPI);

  /* DisplayEnable Enable */
  /* Heltec.LoRa Disable */
  /* Serial Enable */
  /* PABOOST Enable */
  /* long BAND */
  Heltec.begin(true, true, true, true, BAND);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->drawString(0, 10, "Wait for incoming data...");
  Heltec.display->display();
  delay(1000);
  
  //LoRa.onReceive(cbk);
  LoRa.receive();
}

void loop() {
  /* Parâmetros do rádio */
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
  LoRa.setFrequency(BAND);
  LoRa.setSpreadingFactor(spreadingFactor);
  LoRa.setSignalBandwidth(signalBandwidth);
  if(CRC) {
    LoRa.enableCrc();
  } else {
    LoRa.disableCrc();
  }
    
  Serial.println("Analisando pacotes disponíveis!");
  int packetSize = LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  }
  delay(10);
}
