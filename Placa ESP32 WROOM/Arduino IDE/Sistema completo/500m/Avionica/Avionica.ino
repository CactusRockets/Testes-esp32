// 500m

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <SD.h>

#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

#define ALAVANCA 26

#define ENABLE_BUZZER true
#define ENABLE_BMP true
#define ENABLE_MPU true
#define ENABLE_SKIBS true
#define ENABLE_SD true
#define ENABLE_TELEMETRY false
#define ENABLE_GPS true

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
  double latitude, longitude;
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

String soloMessage = "";
String AllDados = "";

bool alavancaAcionada = false;
float initial_altitude;

#include "telemetry.h"
#include "moduleSD.h"
#include "buzzer.h"
#include "BMPMPU.h"
#include "skibs.h"
#include "GPS.h"

void setup() {
  // Inicializa a serial
  Serial.begin(9600);
  Serial.println("");

  pinMode(ALAVANCA, INPUT);

  if(ENABLE_BUZZER) {
    setupBuzzer();
  }

  if(ENABLE_TELEMETRY) {
    setupTelemetry();
  }

  if(ENABLE_SD) {
    setupSd();
  }
  
  if(ENABLE_BMP) {
    setupBMP();
  }

  if(ENABLE_MPU) {
    setupMPU();
  }

  if(ENABLE_SKIBS) {
    setupSkibPins();
  }

  if(ENABLE_GPS) {
    setupGPS();
  }

  allData = {
    { 0,0,0,0,0,0,0,0 },
    { 0,0 }
  };
  soloData = { 0 };

  
  delay(1000);
}

void loop() {
  if(digitalRead(ALAVANCA) == HIGH) {
    if(alavancaAcionada == false) {
      initial_altitude = bmp.readAltitude(1017.3);
    }
    alavancaAcionada = true;
    // Armazena o tempo do microcontrolador
    allData.data.time = millis() / 1000.0;

    // Medições BMP280
    if(ENABLE_BMP) {
      readBMP();
    }

    // Medições MPU6050
    if(ENABLE_MPU) {
      readMPU();
    }

    // Medições GPS
    if(ENABLE_GPS) {
      updateGPSData();
    }

    analyzeStateOfRocket();
    if(ENABLE_SKIBS) {
      if(isDropping) {
        if(parachuteActivated == false) {
          activateSkibs();
        }
        if(millis() - timeForStage >= DELAY_FOR_STAGE) {
          deactivateSkibs();
        }
      }
    }
    if(parachuteActivated) {
      allData.data.parachute = 1;
      if(ENABLE_BUZZER) {
        activateBuzzer();
      }
    }

    // Armazena os dados em uma string
    String dados = String(allData.data.time, 3)         
      + "," + String(allData.data.temperature, 3)       
      + "," + String(allData.data.altitude, 3)
      + "," + String(allData.data.variation_altitude, 3)
      + "," + String(allData.data.acceleration_Z, 3)    
      + "," + String(allData.data.altitude_MPU, 3)      
      + "," + String(allData.data.pressure, 3)
      + "," + String(allData.data.parachute, 3);     
    String GPSDados = String(allData.GPSData.latitude, 3)
      + "," + String(allData.GPSData.longitude, 3);
    AllDados = dados + "," + GPSDados;

    Serial.println(AllDados);

    if(ENABLE_SD) {
      writeOnSD(AllDados);
    }

    if(ENABLE_TELEMETRY) {
      if(LORA_WAY == LORA_STRING_METHOD) {
        transmitString(AllDados);

      } else if(LORA_WAY == LORA_STRUCT_METHOD) {
        transmit(&allData);

      }
      if(LoRaSerial.available() > 0) {
        if(LORA_WAY == LORA_STRING_METHOD) {
          receiveString();

        } else if(LORA_WAY == LORA_STRUCT_METHOD) {
          receive(&soloData);

        }
      }
    }

    // É bom que seja um múltiplo de 500 milisegundos
    delay(500);
  } 
}
