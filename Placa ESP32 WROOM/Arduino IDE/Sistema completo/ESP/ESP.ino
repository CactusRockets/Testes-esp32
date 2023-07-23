#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SD.h>

#define ENABLE_BUZZER true
#define ENABLE_BMP true
#define ENABLE_MPU true
#define ENABLE_SKIB true
#define ENABLE_SD true
#define ENABLE_TELEMETRY true
#define ENABLE_GPS true

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

struct PacketGPSData {
  long latitude, longitude;
  unsigned long idadeInformacaoGPS;
  int ano;
  byte mes, dia, hora, minuto, segundo, centesimo;
  float velocidadeGPS;
  float altitudeGPS;
  // sentido do movimento (em centesimo de graus)
  unsigned long sentidoGPS;
  // Quantidade de satélite da informação
  unsigned short satelitesGPS;
  // Precisão dos dados
  unsigned long precisaoGPS;
};

struct AllPacketData {
  PacketData data;
  PacketGPSData GPSData;
};

struct SoloData {
  int parachute = 0; 
};

AllPacketData allData;
SoloData receivedData;

float initial_altitude;

#include "telemetry.h"
#include "moduleSD.h"
#include "buzzer.h"
#include "BMPMPU.h"
#include "skibs.h"
#include "GPS.h"

void setup() {
  // Inicializa a serial
  Serial.begin(115200);

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

  // Zera os dados da struct
  allData = {
    { 0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
  };
  receivedData.parachute = 0;

  initial_altitude = bmp.readAltitude(1017.3);
  delay(100);
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
  String dados = String(allData.data.time, 3)         
    + "," + String(allData.data.temperature, 3)       
    + "," + String(allData.data.altitude, 3)          
    + "," + String(allData.data.variation_altitude, 3)
    + "," + String(allData.data.acceleration_Z, 3)    
    + "," + String(allData.data.altitude_MPU, 3)      
    + "," + String(allData.data.pressure, 3)
    + "," + String(allData.data.parachute);     
  Serial.println("Dados:" + dados);
  String GPSDados = String(allData.GPSData.latitude, 3)
    + "," + String(allData.GPSData.longitude, 3)
    + "," + String(allData.GPSData.idadeInformacaoGPS, 3)
    + "," + String(allData.GPSData.ano, 3)
    + "," + String(allData.GPSData.mes, 3)
    + "," + String(allData.GPSData.dia, 3)
    + "," + String(allData.GPSData.hora, 3)
    + "," + String(allData.GPSData.minuto, 3)
    + "," + String(allData.GPSData.segundo, 3)
    + "," + String(allData.GPSData.centesimo, 3)
    + "," + String(allData.GPSData.velocidadeGPS, 3)
    + "," + String(allData.GPSData.altitudeGPS, 3)
    + "," + String(allData.GPSData.sentidoGPS, 3)
    + "," + String(allData.GPSData.satelitesGPS, 3)
    + "," + String(allData.GPSData.precisaoGPS, 3);
  Serial.println("Dados do GPS:" + GPSDados);
  String AllDados = dados + "," + GPSDados;

  if(ENABLE_SD) {
    writeOnSD(AllDados);
  }

  if(ENABLE_TELEMETRY) {
    transmit();
    receive();
  }

  delay(50);  
}
