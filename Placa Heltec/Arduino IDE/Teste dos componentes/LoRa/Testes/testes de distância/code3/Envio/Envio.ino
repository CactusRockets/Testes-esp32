
/*
  This is a simple example show the Heltec. LoRa sended data in OLED.

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
#define spreadingFactor 12
// Se refere a quantidade máxima de bits/s que determinada transmissão suporta
#define signalBandwidth 250E3
// O CRC é uma técnica usada para detectar erros em dados digitais
#define CRC 1
// A tecnologia LoRa usa uma taxa de codificação para fornecer correção de erros (FEC)
// e melhorar a confiabilidade da transmissão. Taxas de codificação mais altas fornecem
// melhor correção de erros, mas resultam em taxas de dados mais baixas.
// Em um ambiente móvel, é recomendável usar taxas de codificação mais 
// altas para melhorar a confiabilidade.
#define codingRateDenominator 5
// A tecnologia LoRa usa uma potência de transmissão variável para controlar o alcance
// e a confiabilidade da transmissão. A potência de transmissão mais alta fornece maior
// alcance e resistência à interferência, mas pode consumir mais energia e causar problemas
// de conformidade regulatória. Em um ambiente móvel, é recomendável usar a potência de
// transmissão mais baixa que forneça comunicação confiável para economizar energia
// e evitar problemas regulatórios.
#define txPower 17

unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet;

int success = 0;

void logo() {
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void setup() {
  // WIFI Kit series V1 not support Vext control

  // Setup LoRa
  LoRa.setPins(SS, RESET, DIO0);
  LoRa.setSPIFrequency(frequency_SPI);

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
  LoRa.setTxPower(txPower, PA_OUTPUT_PA_BOOST_PIN);
  LoRa.setFrequency(BAND);
  LoRa.setSpreadingFactor(spreadingFactor);
  LoRa.setSignalBandwidth(signalBandwidth);
  if(CRC) {
    LoRa.enableCrc();
  } else {
    LoRa.disableCrc();
  }
  LoRa.setCodingRate4(codingRateDenominator);

  

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
  Heltec.display->display();
  delay(1000);
}

void loop()
{
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(counter));
  Heltec.display->display();

  // Envio de pacote
  LoRa.beginPacket();
  LoRa.print("00000,00000,00000,00000,00000,00000,00000,00000,0,0,0");
  LoRa.print(",");
  LoRa.print(counter);
  success = LoRa.endPacket();
  if(success) {
    Serial.println("Pacote enviado!");
  } else {
    Serial.println("Pacote não enviado!");
  }

  counter++;
  delay(10);
}
