/* CONFIGURAÇÕES CARTÃO MICROSD */

#include <SD.h>
#define CS_SDPIN 5

File logfile;

/* FUNÇÕES CARTÃO MICROSD */

void setupSD() {
if(!SD.begin(CS_SDPIN)){
    Serial.println("SD not working ...");
    while(1);
  }
  Serial.println("MicroSD Conectado!");
  logfile = SD.open("/dados.txt", FILE_APPEND);

  if(logfile){
    logfile.println("Gravando...");
    Serial.println("Gravando...");
    logfile.close();
  } else {
    Serial.println("Não foi possível gravar...");
  }
}

void writeSd(String text){
  logfile = SD.open("/dados.txt", FILE_APPEND);
  if(logfile) {
    logfile.println(text);
    Serial.println("Gravando...");
    logfile.close();
  } else {
    Serial.println("Não foi possível gravar...");
  }
}