#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SD.h>
#include <HardwareSerial.h>

#define ENABLE_BUZZER false
#define ENABLE_BMP true
#define ENABLE_MPU true
#define ENABLE_SKIB false
#define ENABLE_SD false
#define ENABLE_TELEMETRY true
#define ENABLE_GPS false

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

struct PacketGPSData {
  long latitude, longitude;
};

struct AllPacketData {
  PacketData data;
  PacketGPSData GPSData;
};

struct SoloData {
  int parachute; 
};

AllPacketData allData;
SoloData soloData;
String soloMessage;

float initial_altitude;

#include "telemetry.h"
#include "moduleSD.h"
#include "buzzer.h"
#include "BMPMPU.h"
#include "skibs.h"
#include "GPS.h"

void setup() {
  // Inicializa a serial
  Serial.begin(9600);
  Serial.println("");

  if(ENABLE_BUZZER) {
    setupBuzzer();
  }

  if(ENABLE_TELEMETRY) {
    setupTelemetry();
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

  if(ENABLE_GPS) {
    setupGPS();
  }

  allData = {
    { 0,0,0,0,0,0,0,0 },
    { 0,0 }
  };
  soloData = { 0 };

  initial_altitude = bmp.readAltitude(1017.3);
  delay(1000);
}

void loop() {

  // Armazena o tempo do microcontrolador
  allData.data.time = millis() / 1000.0;

  // Medições BMP280
  if(ENABLE_BMP) {
    readBMP();
  }

  // Medições MPU6050
  if(ENABLE_MPU) {
    readMPU();
  }

  // Medições GPS
  if(ENABLE_GPS) {
    updateGPSData();
  }

  analyzeStateOfRocket();
  if(isDropping) {
    activateSkibs();
  }
  if(parachuteActivated) {
    allData.data.parachute = 1;
    activateBuzzer();
  }

  // Armazena os dados em uma string
  String dados = String(allData.data.time)         
    + "," + String(allData.data.temperature)       
    + "," + String(allData.data.altitude)
    + "," + String(allData.data.variation_altitude)
    + "," + String(allData.data.acceleration_Z)    
    + "," + String(allData.data.altitude_MPU)      
    + "," + String(allData.data.pressure)
    + "," + String(allData.data.parachute);     
  String GPSDados = String(allData.GPSData.latitude)
    + "," + String(allData.GPSData.longitude);
  String AllDados = dados + "," + GPSDados;

  Serial.println(AllDados);

  if(ENABLE_SD) {
    writeOnSD(AllDados);
  }

  if(ENABLE_TELEMETRY) {
    transmitString(AllDados);
    if(LoRaSerial.available() > 0) {
      receiveString();
    }
  }

  delay(500);
}
