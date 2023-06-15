/* CONFIGURAÇÕES CARTÃO MICROSD */

#include <SD.h>
#define CS_SDPIN 5

File myFile;

void setupSd() {
  Serial.print("Inicializando o cartão SD...");
  // verifica se o cartão SD está presente e se pode ser inicializado
  
  if (!SD.begin(CS_SDPIN)) {
    // programa encerrado 
    Serial.println("Falha, verifique se o cartão está presente.");
    return;                                                      
  }

  // Cria arquivo data.txt e abre
  myFile = SD.open("/data.txt", FILE_APPEND);                        
  // Escreve dados no arquivo
  if (myFile) {
    Serial.print("Gravando...");
    myFile.println("Tempo, Temperatura, Altitude, VAltitude, AceleraçãoZ, AltitudeMPU, Pressão, Paraquedas");
    myFile.close();

  } else {
    Serial.println("Error ao abrir data.txt");
  }
}

void writeOnSD(String str) {
  myFile = SD.open("/data.txt", FILE_APPEND);

  if (myFile) {
    Serial.println("(OK)");
    myFile.println(str);
    myFile.close();

  } else {
    Serial.println("Error ao gravar em data.txt");
  }
}
