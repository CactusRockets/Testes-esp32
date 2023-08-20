#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SD.h>

#define ENABLE_BUZZER false
#define ENABLE_BMP false
#define ENABLE_MPU false
#define ENABLE_SKIB false
#define ENABLE_SD true
#define ENABLE_NRF false

#define SKIB1 16
#define SKIB2 17

#define CE_PIN 12
#define CS_NRFPIN 2
#define CS_SDPIN 5

#define BMP_ADRESS 0x76
#define MPU_ADRESS 0x68

// O tamanho deste pacote não deve exceder 32 bytes
struct PacketData {
  float time;
  float altitude_MPU;
  float temperature;
  float pressure;
  float altitude;
  float variation_altitude;
  float acceleration_Z;
  int parachute; 
};

PacketData data;
float initial_altitude;

#include "nrf.h"
#include "moduleSD.h"
#include "buzzer.h"
#include "BMPMPU.h"
#include "skibs.h"


void setup() {
  // Inicializa a serial
  Serial.begin(115200);

  if(ENABLE_BUZZER) {
    setupBuzzer();
  }

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
  data.time = 0;
  data.altitude_MPU = 0;
  data.temperature = 0;
  data.pressure = 0;
  data.altitude = 0;
  data.variation_altitude = 0;
  data.acceleration_Z = 0; 

  initial_altitude = bmp.readAltitude(1017.3);
  delay(100);
}

void loop() {

  // Armazena o tempo do microcontrolador
  data.time = millis() / 1000.0;

  // Medições BMP280
  if(ENABLE_BMP) {
    readBMP();
  }

  // Medições MPU6050
  if(ENABLE_MPU) {
    readMPU();
  }

  analyzeStateOfRocket();
  if(isDropping) {
    activateSkibs();
  }
  if(parachuteActivated) {
    data.parachute = 1;
    activateBuzzer();
  }

  // Armazena os dados em uma string
  String dados = String(data.time, 3)
    + "," + String(data.temperature, 3)
    + "," + String(data.altitude, 3)
    + "," + String(data.variation_altitude, 3)
    + "," + String(data.acceleration_Z, 3)
    + "," + String(data.altitude_MPU, 3)
    + "," + String(data.pressure, 3)
    + "," + String(data.parachute);
  Serial.println(dados);

  if(ENABLE_SD) {
    writeOnSD(dados);
  }

  if(ENABLE_NRF) {
    transmit();
    receive();
  }

  delay(50);  
}
