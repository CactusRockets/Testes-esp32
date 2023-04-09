#include "Arduino.h"
#include "heltec.h"
#include "images.h"

#include <Wire.h>
#include <SD.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define BMP_ADRESS 0x76
#define MPU_ADRESS 0x68
#define CS_PIN_SD 17
#define CS_PIN_LORA 18
#define BAND 433E6

#define ENABLE_SERIAL_BEGIN false
#define ENABLE_SD true
#define ENABLE_LORA_OLED true

int contador = 0;
Adafruit_BMP280 bmp;
File logfile;
Adafruit_MPU6050 mpu;

String rssi = "RSSI --";
String packSize = "--";
String packet;

void logo() {
  Heltec.display->clear();
  Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}

void writeSd(String text){
  logfile = SD.open("/meulog.txt", FILE_APPEND);
  if(logfile){
    logfile.println(text);
    Serial.println("Gravando...");
    logfile.close();
  }
}

void setup() {
  if(ENABLE_LORA_OLED) {
    // WIFI Kit series V1 not support Vext control
  
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
  digitalWrite(CS_PIN_LORA, HIGH);

  delay(5000);


  

  if(ENABLE_SERIAL_BEGIN) {
    Serial.begin(115200);
  }



  if(ENABLE_SD) {
    if(!SD.begin(CS_PIN_SD)){
      Serial.println("SD not working ...");
      while(1);
    }
    Serial.println("MicroSD Conectado!");
  }
  digitalWrite(CS_PIN_SD, HIGH);




  if(!bmp.begin(BMP_ADRESS)) {
    Serial.println("BMP not working ...");
    while(1);
  }
  Serial.println("BMP conectado!");

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);





  if (!mpu.begin(MPU_ADRESS)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
}



void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Serial.println(a.acceleration.v[0]);
  Serial.println(a.acceleration.v[1]);
  Serial.println(a.acceleration.v[2]);
  Serial.println(a.acceleration.z);
  Serial.println(g.gyro.pitch);
  Serial.println(g.gyro.roll);




  digitalWrite(CS_PIN_SD, LOW);
  if(ENABLE_SD) {
    writeSd("Alanzinho" + String(contador));
  }
  digitalWrite(CS_PIN_SD, HIGH);



  
  digitalWrite(CS_PIN_LORA, LOW);
  if(ENABLE_LORA_OLED) {

    Heltec.display->clear();
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->setFont(ArialMT_Plain_10);
    
    Heltec.display->drawString(0, 0, "Sending packet: ");
    Heltec.display->drawString(90, 0, String(contador));
    Heltec.display->display();


    
    
    Serial.print("Sending packet: ");
    Serial.println(contador);

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
    LoRa.print(contador);
    LoRa.endPacket();
  }
  digitalWrite(CS_PIN_LORA, HIGH);



  contador++;
  delay(3000);
}
