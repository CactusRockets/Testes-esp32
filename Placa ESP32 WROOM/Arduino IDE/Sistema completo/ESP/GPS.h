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
    gps.crack_datetime(
      &ano, &mes, &dia, &hora, &minuto, &segundo, &centesimo, &idadeInformacaoGPS
    );

    velocidadeGPS = gps.f_speed_kmph();
    altitudeGPS = gps.f_altitude();
    sentidoGPS = gps.course();
    satelitesGPS = gps.satellites();
    precisaoGPS =  gps.hdop();
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

  // Altitude
  if ((altitudeGPS == TinyGPS::GPS_INVALID_ALTITUDE) || (altitudeGPS == 1000000)) {
    altitudeGPS = 0;
  }

  // Satélites e Precisão
  if (satelitesGPS == TinyGPS::GPS_INVALID_SATELLITES) {
    satelitesGPS = 0;
  }
  if (precisaoGPS == TinyGPS::GPS_INVALID_HDOP) {
    precisaoGPS = 0;
  }
}

void saveGPSData() {
  allData.GPSData = {
    latitude, longitude,
    idadeInformacaoGPS,
    ano,
    mes, dia, hora, minuto, segundo, centesimo,
    velocidadeGPS,
    altitudeGPS,
    // sentido do movimento (em centesimo de graus)
    sentidoGPS,
    // Quantidade de satélite da informação
    satelitesGPS,
    // Precisão dos dados
    precisaoGPS
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