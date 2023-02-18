#include <Arduino.h>
#include <Wire.h>
#include <SD.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "heltec.h"

#define BMP_ADRESS 0x76
#define MPU_ADRESS 0x68
#define CS_PIN 17
#define BAND 433E6

#define ENABLE_SERIAL_BEGIN true
#define ENABLE_SD true
#define ENABLE_LORA_OLED false

int contador = 0;
Adafruit_BMP280 bmp;
File logfile;
Adafruit_MPU6050 mpu;

void writeSd(String text){
  logfile = SD.open("/meulog.txt", FILE_APPEND);
  if(logfile){
    logfile.println(text);
    Serial.println("Gravando...");
    logfile.close();
  }
}

void setup() {
  if(ENABLE_SERIAL_BEGIN) {
    Serial.begin(115200);
  }


  if(ENABLE_SD) {
    if(!SD.begin(CS_PIN)){
      Serial.println("SD not working ...");
      while(1);
    }
    Serial.println("MicroSD Conectado!");
  }


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



  if(ENABLE_LORA_OLED) {
    /* DisplayEnable Enable */
    /* Heltec.LoRa Disable */
    /* Serial Enable */
    /* PABOOST Enable */
    /* long BAND */
    Heltec.begin(true, true, true, true, BAND);
  }
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


  if(ENABLE_SD) {
    writeSd("Alanzinho" + String(contador));
  }


  if(ENABLE_LORA_OLED) {
    Serial.print("Sending packet: ");
    Serial.println(contador);

    LoRa.beginPacket();
    LoRa.setTxPower(14, RF_PACONFIG_PASELECT_PABOOST);
    LoRa.print("hello ");
    LoRa.print(contador);
    LoRa.endPacket();
  }



  contador++;
  delay(3000);
}
