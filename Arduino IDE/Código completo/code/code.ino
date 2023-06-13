#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SD.h>

#define ENABLE_BMP true
#define ENABLE_MPU true
#define ENABLE_SKIB true
#define ENABLE_SD true
#define ENABLE_NRF true

#define SKIB1 16
#define SKIB2 17

#define CE_PIN 12
#define CS_NRFPIN 2
#define CS_SDPIN 5

#define BMP_ADRESS 0x76
#define MPU_ADRESS 0x68

// O tamanho deste pacote não deve exceder 32 bytes
struct PacketData {
  unsigned long mill;
  float altmp;
  float temp;
  float pres;
  float alt;
  float vaalt;
  float acz;
  int parachute;
  // 4+4+4+4+4+4+4+4=32bytes  
};

PacketData data;
float altini;

#include "nrf.h"
#include "moduleSD.h"
#include "buzzer.h"
#include "BMPMPU.h"
#include "skibs.h"


void setup() {
  // Inicializa a serial
  Serial.begin(115200);

  if(ENABLE_NRF) {
    setupNRF();
  }

  if(ENABLE_SD) {
    setupSd();
  }
  
  if(ENABLE_BMP) {
    setupBMP();
  }

  if(ENABLE_MPU) {
    setupMPU();
  }

  if(ENABLE_SKIB) {
    setupSkibPins();
  }

  // Zera os dados da struct
  data.mill = 0;
  data.altmp = 0;
  data.temp = 0;
  data.pres = 0;
  data.alt = 0;
  data.vaalt = 0;
  data.acz = 0; 

  altini = bmp.readAltitude(1017.3);
  delay(100);
}

void loop() {

  // Armazena o tempo do microcontrolador
  data.mill= millis()/1000;

  // Medições BMP280
  if(ENABLE_BMP) {
    testBMP();
  }

  // Medições MPU6050
  if(ENABLE_MPU) {
    testMPU();
  } 

  // Armazena os dados em uma string
  String dados = String(data.mill,3)
    + "," + String(data.temp, 3)
    + "," + String(data.alt, 3)
    + "," + String(data.vaalt, 3)
    + "," + String(data.acz, 3)
    + "," + String(data.altmp, 3)
    + "," + String(data.pres, 3)
    + "," + String(data.parachute);
  Serial.println(dados);

  if(ENABLE_SD) {
    writeOnSD(dados);
  }

  if(ENABLE_NRF) {
    transmit();
  }

  delay(50);  
}
