/* CONFIGURAÇÕES CARTÃO MICROSD */

#define CS_SDPIN 5

File myFile;

void setupSd() {
  Serial.println("Inicializando o cartão SD...");
  // verifica se o cartão SD está presente e se pode ser inicializado
  
  while(!SD.begin(CS_SDPIN)) {
    // programa encerrado 
    activateBuzzer();
    Serial.println("Falha, verifique se o cartão está presente.");
  }

  // Cria arquivo data.txt e abre
  myFile = SD.open("/data.txt", FILE_APPEND);                        
  // Escreve dados no arquivo
  if (myFile) {
    Serial.println("Gravando dados iniciais!");
    myFile.println("Informations");
    myFile.close();

  } else {
    Serial.println("Error ao abrir data.txt");
  }
}

void writeOnSD(String str) {
  myFile = SD.open("/data.txt", FILE_APPEND);

  if (myFile) {
    Serial.println("(Dados Gravados!)");
    myFile.println(str);
    myFile.close();

  } else {
    Serial.println("Error ao gravar em data.txt");
  }
}
