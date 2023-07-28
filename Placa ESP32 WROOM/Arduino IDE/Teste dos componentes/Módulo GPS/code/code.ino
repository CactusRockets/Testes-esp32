
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
  // Sentido do movimento (em centesimo de graus)
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
  int parachute; 
};

AllPacketData allData;
SoloData soloData;

#include "GPS.h"

void clearData() {
  allData = {
    { 0,0,0,0,0,0,0,0 },
    { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
  };
  soloData = { 0 };
}

void printData() {
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
}


void setup() {
  Serial.begin(9600);

  setupGPS();
  clearData();
}

void loop() {
  
  getGPSData();
  saveGPSData();
  printData();

  delay(200);
}
