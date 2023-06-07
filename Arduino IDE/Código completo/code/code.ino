#include "nrf.h"
#include "moduleSD.h"
#include "BMPMPU.h"
#include "skibs.h"
#include "KalmanAltitude.h"
#include "buzzer.h"

#define ENABLE_BMP true
#define ENABLE_MPU true
#define ENABLE_SD true
#define ENABLE_NRF true
#define ENABLE_SKIB true
#define ENABLE_BUZZER true

KalmanAltitude kalmanFilter;
float deltaTime = 0;
int contador = 0;

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
    setupSkibPins();
  }

  if(ENABLE_BUZZER) {
    setupBuzzer();
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

  // Variação do tempo (em segundos)
  deltaTime = (millis() - deltaTime) * 1000;
  kalmanFilter.update(altitudeAtual, velocidadeAtual, deltaTime);
  
  // Verificação da ativação do paraquedas
  if(ENABLE_SKIB) {
    analyzeStateOfRocket();
    
    if(isDropping) {
      activateSkibs();
      activateBuzzer();
    }
  }

  // Preparação para armazenamento dos dados
  String tempoString = String(millis(), 3);
  String altitudeString = String(kalmanFilter.Altitude, 3);
  String velocityString = String(kalmanFilter.Velocity, 3);
  String parachuteString = String(parachuteActivated ? "1": "0");

  String payloadString = tempoString+","+altitudeString+","+velocityString+","+parachuteString;

  // Enviar dados para o cartão SD
  if(ENABLE_SD) {
    digitalWrite(CS_SDPIN, LOW);
    writeSd(payloadString);
    digitalWrite(CS_SDPIN, HIGH);
  }

  // Enviar dados para a telemetria
  // Receber dados da telemetria
  if(ENABLE_NRF) {
    digitalWrite(CS_NRFPIN, LOW);

    updateNRF();
    changeRoleNRF(false);
    updateNRF();
    changeRoleNRF(true);
    
    digitalWrite(CS_NRFPIN, HIGH);
  }

  // Ativação manual dos skibs
  if(ENABLE_SKIB) {
    if(payloadReceived) {
      activateSkibs();
      activateBuzzer();
    }
  }

  if(ENABLE_BUZZER) {
    activateBuzzer();
  }
  
  
  contador++;
  Serial.println(String("Quantidade no Loop: ") + String(contador));
  delay(150);
}
