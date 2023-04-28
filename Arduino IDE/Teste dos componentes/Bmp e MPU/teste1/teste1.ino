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


  Serial.println("--------------------------------");
  

  Serial.println("MPU:");
  Serial.println(a.acceleration.v[0]);
  Serial.println(a.acceleration.v[1]);
  Serial.println(a.acceleration.v[2]);
  Serial.println(a.acceleration.z);
  Serial.println(g.gyro.pitch);
  Serial.println(g.gyro.roll);
  Serial.println();

  

  float number = bmp.readAltitude(1013);
  Serial.println("BMP:");
  Serial.println(number);
  Serial.println();



  contador++;
  delay(3000);
}
