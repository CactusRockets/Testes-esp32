#include "Arduino.h"
#include "heltec.h"

#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define BMP_ADRESS 0x76
#define MPU_ADRESS 0x68
#define CS_PIN_LORA 18
#define BAND 433E6

#define ENABLE_SERIAL_BEGIN true
#define ENABLE_LORA true

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





  if(ENABLE_LORA) {
    /* DisplayEnable Enable */
    /* Heltec.LoRa Disable */
    /* Serial Enable */
    /* PABOOST Enable */
    /* long BAND */
    Heltec.begin(false, true, true, true, BAND);
    Serial.println("LoRa inicializado!");
  }
  digitalWrite(CS_PIN_LORA, HIGH);
}



void loop() {
  float t0,t1,t2,t3,t4,t5;  
  t0 = millis();
  
  sensors_event_t a, g, temp;
  t1 = millis();
  mpu.getEvent(&a, &g, &temp);
  t2 = millis();

  /*
  Serial.print(t2 - t1);
  Serial.print(",");
  Serial.print(a.acceleration.v[0]);
  Serial.print(",");
  Serial.print(a.acceleration.v[1]);
  Serial.print(",");
  Serial.print(a.acceleration.v[2]);
  Serial.print(",");
  Serial.print(a.acceleration.z);
  Serial.print(",");
  Serial.print(g.gyro.pitch);
  Serial.print(",");
  Serial.print(g.gyro.roll);
  Serial.print(",");
  Serial.println(contador);
  */
  
  digitalWrite(CS_PIN_LORA, LOW);
  if(ENABLE_LORA) {

    // Setup
    LoRa.setTxPower(14, RF_PACONFIG_PASELECT_PABOOST);

    t3 = millis();
    
    // Envio de pacote
    LoRa.beginPacket();
    LoRa.print(t0);
    LoRa.print(",");
    LoRa.print(t2 - t1);
    LoRa.print(",");
    LoRa.print(a.acceleration.v[0]);
    LoRa.print(",");
    LoRa.print(a.acceleration.v[1]);
    LoRa.print(",");
    LoRa.print(a.acceleration.v[2]);
    LoRa.print(",");
    LoRa.print(a.acceleration.z);
    LoRa.print(",");
    LoRa.print(g.gyro.pitch);
    LoRa.print(",");
    LoRa.print(g.gyro.roll);
    LoRa.print(",");
    LoRa.println(contador);
    LoRa.endPacket();

    t4 = millis();
  }
  digitalWrite(CS_PIN_LORA, HIGH);

  contador++;

  Serial.println("Tempo atual:" + String(t0));
  Serial.println("Tempo para ler informações dos sensores:" + String(t2 - t1));
  Serial.println("Tempo de envio de pacote:" + String(t4 - t3));
  Serial.println("");
  delay(10000);
}
