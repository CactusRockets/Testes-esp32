#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

static const int RXPin = 15, TXPin = 4;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
HardwareSerial GPSSerial(1);

double latitude, longitude;
int ano, mes, dia, hora, minuto, segundo, centesimo;

void setup()
{
  Serial.begin(9600);
  GPSSerial.begin(GPSBaud);
}

void loop()
{
  bool isNewGPSData = false;

  while(GPSSerial.available() > 0) {
    char GPSData = GPSSerial.read();
    isNewGPSData = gps.encode(GPSData);
  }

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

  printData();
}

void printData()
{
  Serial.print("Latitude: ");
  Serial.print(latitude, 6);
  Serial.print(",");
  Serial.print("Longitude: ");
  Serial.print(longitude, 6);
  Serial.println();

  Serial.print(dia);
  Serial.print("/");
  Serial.print(mes);
  Serial.print("/");
  Serial.print(ano);
  Serial.print(" | ");
  Serial.print(hora);
  Serial.print("/");
  Serial.print(minuto);
  Serial.print("/");
  Serial.print(segundo);
  Serial.print("/");
  Serial.print(centesimo);

  Serial.println();
}
