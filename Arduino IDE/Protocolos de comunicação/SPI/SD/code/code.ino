#include <SPI.h>

/* CONFIGURAÇÕES PARA O USO DO CARTÃO SD */
#include <SD.h>

#define SCK_PIN 18
#define MISO_PIN 19
#define MOSI_PIN 23
#define CS_SDPIN 5

SPIClass vspi(VSPI);
/*
SPIClass hspi(HSPI);
*/

File logfile;
int contador = 0;

void writeSd(String text){
  logfile = SD.open("/meulog.txt", FILE_APPEND);
  if(logfile){
    logfile.println(text);
    Serial.println("Gravando...");
    logfile.close();
  } else {
    Serial.println("Não foi possível gravar...");
  }
}

void setupSD() {
  while(!SD.begin(CS_SDPIN)){
    Serial.println("SD not working ...");
  }
  Serial.println("MicroSD Conectado!");
}

void setup() {
  Serial.begin(115200);
  /*
  vspi.begin(SCK_PIN,SPIClass spi(VSPI); MISO_PIN, MOSI_PIN, CS_NRFPIN);
  hspi.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_SDPIN);
  */
  pinMode(CS_SDPIN, OUTPUT);
  digitalWrite(CS_SDPIN, HIGH);

  vspi.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_SDPIN);

  setupSD();
}

void loop() {
  writeSd("Alanzinho" + String(contador));

  contador++;
  delay(300);
}
