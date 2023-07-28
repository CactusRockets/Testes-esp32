/* CONFIGURAÇÕES GPS */

#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

// Pinos da Serial 1 do ESP32
#define RX1_PIN 15
#define TX1_PIN 4

// Usando a Serial 1 do ESP32
HardwareSerial GPSSerial(1);
TinyGPSPlus gps;

double latitude, longitude;
int ano, mes, dia, hora, minuto, segundo, centesimo;

float velocidadeGPS;
float altitudeGPS;
// sentido do movimento (em centesimo de graus)
unsigned long sentidoGPS;
// Quantidade de satélite da informação
unsigned short satelitesGPS;
// Precisão dos dados
unsigned long precisaoGPS;
unsigned long idadeInformacaoGPS;


void setupGPS() {
  // Configuração inicial do GPS
  GPSSerial.begin(9600);
  while(!GPSSerial);
}

void getGPSData() {
  bool isNewGPSData = false;

  while(GPSSerial.available() > 0) {
    char GPSData = GPSSerial.read();
    isNewGPSData = gps.encode(GPSData);
  }
  Serial.println();

  if(isNewGPSData) {
    if(gps.location.isValid()) {
      latitude = gps.location.lat();
      longitude = gps.location.lng();
    }

    if(gps.date.isValid()) {
      mes = gps.date.month();
      dia = gps.date.day();
      ano = gps.date.year();
    }
    if(gps.time.isValid()) {
      hora = gps.time.hour();
      minuto = gps.time.minute();
      segundo = gps.time.second();
      centesimo = gps.time.centisecond();
    }
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
