#define ENABLE_BMP true
#define ENABLE_MPU true
#define ENABLE_SD true
#define ENABLE_NRF true
#define ENABLE_SKIB true

/* CONFIGURAÇÕES NRF24L01 */

#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include "nRF24L01.h"

#define SCK_PIN 18
#define MISO_PIN 19
#define MOSI_PIN 23

#define CS_NRFPIN 12
#define CE_PIN 2

SPIClass vspi(VSPI);
SPIClass hspi(HSPI);

RF24 radio(CE_PIN, CS_NRFPIN);


// ANALISAR
uint8_t address[][6] = {"1Node", "2Node"};
bool radioNumber = 1;



// Usado para controlar funcionalidade do módulo rádio
// True = Tx role, False = RX role
bool role = true;
String payload = "000000,000000";
bool payloadReceived = false;

/* FUNÇÕES NRF */

void setupNRF() {
  delay(100);
  while(!radio.begin()) {
    Serial.println(F("Radio hardware is not responding!!"));
    delay(100);
  }

  // ANALISAR
  delay(100);
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(sizeof(payload));
  radio.openWritingPipe(address[radioNumber]);     
  radio.openReadingPipe(1, address[!radioNumber]); 

  if(role) {
    radio.stopListening();  
  } else {
    radio.startListening(); 
  }
}

// ANALISAR
void updateNRF() {
  radio.setPayloadSize(sizeof(payload));
  
  if(role) {
    // Funcionalidade de transmissor

    unsigned long start_timer = micros();                 
    bool report = radio.write(&payload, sizeof(payload));
    unsigned long end_timer = micros();                      

    if(report) {
      Serial.print(F("Transmission successful! "));
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);
      Serial.println(F("us"));

      Serial.println("Pacote:" + String(payload));
    } else {
      Serial.println(F("Transmission failed or timed out"));
    }

  } else {
    // Funcionalidade de receptor
    
    uint8_t pipe;
    if (radio.available(&pipe)) {
      uint8_t bytes = radio.getPayloadSize();
      radio.read(&payloadReceived, bytes);
      
      Serial.print(F("Received "));
      Serial.print(bytes);
      Serial.print(F(" bytes on pipe "));
      Serial.println(pipe);
      
      Serial.print(F("Pacote:"));
      Serial.println(payloadReceived);
    }
  }

  if (Serial.available()) {
    // change the role via the serial monitor

    char c = toupper(Serial.read());
    if (c == 'T' && !role) {
      // Become the TX node
      role = true;
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      radio.stopListening();

    } else if (c == 'R' && role) {
      // Become the RX node
      role = false;
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
      radio.startListening();
    }
  }
}


/* CONFIGURAÇÕES CARTÃO MICROSD */
#include <SD.h>

#define CS_SDPIN 5

File logfile;
int contador = 0;

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

/* CONFIGURAÇÕES BMP e MPU */
#include "Arduino.h"

#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define BMP_ADRESS 0x76
#define MPU_ADRESS 0x68

Adafruit_BMP280 bmp;
Adafruit_MPU6050 mpu;

/* FUNÇÕES BMP280 */

double altitudeAtual = 0;
double altitudeAnterior = 0;

void setupBMP() {
  if(!bmp.begin(BMP_ADRESS)) {
    Serial.println("BMP not working ...");
    while(1);
  }
  Serial.println("BMP conectado!");

  // ANALISAR
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);
}

String testBMP() {
  float number = bmp.readAltitude(1013);
  Serial.println("---------------------------------");
  Serial.println(String("BMP:") + String(number));
  Serial.println("---------------------------------");

  return String(number);
}

// ANALISAR
void readAltitudeBMP() {
  altitudeAtual = bmp.readAltitude(1013);
  
  if(altitudeAtual > maximumAltitudeValue) {
    maximumAltitudeValue = altitudeAtual;
  }
}

/* FUNÇÕES MPU6050 */

double aceleracaoAtual = 0;
double aceleracaoAnterior = 0;

double velocidadeAtual = 0;
double velocidadeAnterior = 0;

void setupMPU() {
  if (!mpu.begin(MPU_ADRESS)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(100);
    }
  }
  Serial.println("MPU6050 Found!");

  // ANALISAR 
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

String testMPU() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  Serial.println("---------------------------------");
  Serial.println("MPU:");
  Serial.println(a.acceleration.v[0]);
  Serial.println(a.acceleration.v[1]);
  Serial.println(a.acceleration.v[2]);
  Serial.println(a.acceleration.z);
  Serial.println(g.gyro.pitch);
  Serial.println(g.gyro.roll);
  Serial.println("---------------------------------");

  return String(a.acceleration.v[0]) + "," + String(a.acceleration.v[1]) + "," + String(a.acceleration.v[2]);
}

// ANALISAR
void readVelocityMPU() {
   velocidadeAtual = 42.0;
}

/* CONFIGURAÇÕES SKIB */
#define SKIB1 16
#define SKIB2 17
#define rangeMaximumALtitudeValue 5

double maximumAltitudeValue = 0;

void setupSkib() {
  pinMode(SKIB1, OUTPUT);
  pinMode(SKIB2, OUTPUT);
}

void executeSkib() {
  if((maximumAltitudeValue - altitudeAtual) > rangeMaximumValue) {
    Serial.println("Primeiro skib ativado!");
    digitalWrite(SKIB1, HIGH);
    Serial.println("Segundo skib ativado!");
    digitalWrite(SKIB2, HIGH);
  }
}

/* COMFIGURAÇÕES FILTRO DE KALMAN */
#include "KalmanAltitude.h"

KalmanAltitude kalmanFilter();
float deltaTime = 0;



void setup() {
  Serial.begin(115200);
  while (!Serial) {}
  
  pinMode(CS_NRFPIN, OUTPUT);
  pinMode(CS_SDPIN, OUTPUT);
  
  
  if(ENABLE_NRF) {
    digitalWrite(CS_NRFPIN, LOW);
    setupNRF();
    digitalWrite(CS_NRFPIN, HIGH);
  }

  if(ENABLE_SD) {
    digitalWrite(CS_SDPIN, LOW);
    setupSD();
    digitalWrite(CS_SDPIN, HIGH);
  }

  if(ENABLE_BMP) {
    setupBMP();
  }

  if(ENABLE_MPU) {
    setupMPU();
  }

  if(ENABLE_SKIB) {
    setupSkib();
  }
}

void loop() {
  // Leitura das variáveis necessárias
  if(ENABLE_BMP) {
    readAltitudeBMP();
  }
  if(ENABLE_MPU) {
    readVelocityMPU();
  }
  deltaTime = millis();
  kalmanFilter.update(altitudeAtual, velocidadeAtual, deltaTime);
  
  // Verificação da ativação do paraquedas
  if(ENABLE_SKIB) {
    executeSkib();
  }

  // Preparação para armazenamento dos dados
  String altitudeString = String(kalmanFilter.Altitude, 3);
  String velocityString = String(kalmanFilter.Velocity, 3);

  payload = altitudeString + "," + velocityString;

  // Enviar dados para o cartão SD
  if(ENABLE_SD) {
    digitalWrite(CS_SDPIN, LOW);
    writeSd(payload);
    digitalWrite(CS_SDPIN, HIGH);
  }

  // Enviar dados para a telemetria
  if(ENABLE_NRF) {
    digitalWrite(CS_NRFPIN, LOW);
    updateNRF();
    digitalWrite(CS_NRFPIN, HIGH);
  }
  
  contador++;
  Serial.println(String("Quantidade no Loop: ") + String(contador));
  delay(150);
}
