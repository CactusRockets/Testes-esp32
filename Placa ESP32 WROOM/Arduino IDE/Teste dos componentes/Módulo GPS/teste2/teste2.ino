//INCLUSÃO DE BIBLIOTECAS
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

/*
DEFINIÇÃO DE PINOS
*/
int RXPin = 15;
int TXPin = 4;

int GPSBaud = 9600;

// CRIANDO UM O BJETO PARA COMUNICAR COM A BIBLIOTECA
TinyGPSPlus gps;

// CRIANDO UMA PORTA SERIAL gpsSerial PARA CONVERSAR COM MÓDULO
HardwareSerial gpsSerial(1);

void setup()
{
  // INICIA A SERIAL
  Serial.begin(9600);

  // Configuração inicial do GPS
  gpsSerial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);
}

void loop()
{
  // TODA VEZ QUE FOR LIDA UMA NOVA SENTENÇA NMEA, CHAMAREMOS A FUNÇÃO displayInfo() PARA MOSTRAR OS DADOS NA TELA
  while(gpsSerial.available() > 0) {

    char value = gpsSerial.read();
    Serial.print(value);

    if(gps.encode(value)) {
      displayInfo();
    }
  }

  // SE EM ALGUNS SEGUNDOS NÃO FOR DETECTADA NENHUMA NOVA LEITURA PELO MÓDULO, 
  // SERÁ MOSTRADO ESTA MENSAGEM DE ERRO.
  if (millis() > 15000 && gps.charsProcessed() < 10) {
    Serial.println("\nSinal GPS não detectado");
    while(true);
  }
}

// FUNÇÃO RESPONSAVEL PELA LEITURA DOS DADOS
void displayInfo() {
  // SE A LOCALIZAÇÃO DO SINAL ENCONTRADO É VÁLIDA, ENTÃO
  if(gps.location.isValid()) {
    Serial.print("\nLatitude: ");

    // IMPRIME NA SERIAL O VALOR DA LATITUDE LIDA
    Serial.println(gps.location.lat(), 6);
    Serial.print("\nLongitude: ");

    // IMPRIME NA SERIAL O VALOR DA LONGITUDE LIDA
    Serial.println(gps.location.lng(), 6);
  } else {
    // SE NÃO HOUVER NENHUMA LEITURA, IMPRIME A MENSAGEM DE ERRO NA SERIAL
    Serial.println("\nNão detectamos a localização");
  }

  Serial.print("Data: ");
  // IMPRIME A DATA NA SERIAL
  if(gps.date.isValid()) {
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.println(gps.date.year());
  } else {
    // SE NÃO HOUVER NENHUMA LEITURA, IMPRIME A MENSAGEM DE ERRO NA SERIAL
    Serial.println("Erro");
  }

  // LEITURA DA HORA PARA SER IMPRESSA NA SERIAL
  Serial.print("Time: ");
  if(gps.time.isValid()) {

    if (gps.time.hour() < 10)
      Serial.print(F("0"));

    // AJUSTA O FUSO HORARIO PARA NOSSA REGIAO (FUSO DE SP 03:00, POR ISSO O -3 NO CÓDIGO) E IMPRIME NA SERIAL
    Serial.print(gps.time.hour() - 3); 
    Serial.print(":");

    if(gps.time.minute() < 10) 
      Serial.print(F("0"));

    // IMPRIME A INFORMAÇÃO DOS MINUTOS NA SERIAL
    Serial.print(gps.time.minute());
    Serial.print(":");

    if(gps.time.second() < 10) 
      Serial.print(F("0"));

    // IMPRIME A INFORMAÇÃO DOS SEGUNDOS NA SERIAL
    Serial.print(gps.time.second());

  } else {
    // SE NÃO HOUVER NENHUMA LEITURA, IMPRIME A MENSAGEM DE ERRO NA SERIAL
    Serial.println("Não detectamos o horário atual");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}
