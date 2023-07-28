/* CONFIGURAÇÕES GPS */

#include <HardwareSerial.h>
#include <TinyGPS.h>

// Pinos da Serial 3 do ESP32
#define RX1_PIN 15
#define TX1_PIN 4

// Usando a Serial 3 do ESP32
HardwareSerial GPSSerial(1);
TinyGPS gps;


long latitude, longitude;
unsigned long idadeInformacaoGPS;

void setupGPS() {
  //Configuração inicial do GPS
  GPSSerial.begin(9600, SERIAL_8N1, RX1_PIN, TX1_PIN);
  while(!GPSSerial);
}

void getGPSData() {
  bool isNewGPSData = false;

  while(GPSSerial.available()) {
    char GPSData = GPSSerial.read();
    isNewGPSData = gps.encode(GPSData);
  }

  if(isNewGPSData) {
    gps.get_position(&latitude, &longitude, &idadeInformacaoGPS);
  }
}

void organizeGPSData() {

  // Latitude e Longitude
  if (latitude == TinyGPS::GPS_INVALID_F_ANGLE) {
    latitude = 0;
  }
  if (longitude == TinyGPS::GPS_INVALID_F_ANGLE) {
    longitude = 0;
  }
  if (idadeInformacaoGPS != TinyGPS::GPS_INVALID_AGE) {
    idadeInformacaoGPS = 0;
  }
}

void saveGPSData() {
  allData.GPSData = {
    latitude, longitude
  };
}

void updateGPSData() {
  getGPSData();
  organizeGPSData();
  saveGPSData();
}

float distanceBetweenCoodinates(
  long latitude1, long longitude1, long latitude2, long longitude2
) {
  return gps.distance_between(latitude1, longitude1, latitude2, longitude2);
}

float courseBetweenCoodinates(
  long latitude1, long longitude1, long latitude2, long longitude2
) {
  return gps.course_to(latitude1, longitude1, latitude2, longitude2);
}