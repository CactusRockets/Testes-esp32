#include "Arduino.h"

#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define BMP_ADRESS 0x76
#define MPU_ADRESS 0x68

#define ENABLE_SERIAL_BEGIN true

int contador = 0;
Adafruit_BMP280 bmp;
Adafruit_MPU6050 mpu;


void setup() {
  if(ENABLE_SERIAL_BEGIN) {
    Serial.begin(115200);
  }






  
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
  

}



void loop() {

  float number = bmp.readAltitude(1013);
  Serial.println(String("BMP:") + String(number));


  contador++;
  delay(3000);
}
