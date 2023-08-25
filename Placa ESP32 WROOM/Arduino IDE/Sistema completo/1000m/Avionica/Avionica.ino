// 1Km

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_BMP3XX.h>
#include <Adafruit_Sensor.h>
#include "MPU.h"
#include <SD.h>

#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

#define ENABLE_BUZZER true
#define ENABLE_BMP true
#define ENABLE_MPU false
#define ENABLE_SKIBS true
#define ENABLE_SD true
#define ENABLE_TELEMETRY false
#define ENABLE_GPS true

#define ALAVANCA 26

struct PacketData {
  float time;
  float temperature;
  float pressure;
  float altitude;
  float variationAltitude;
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

#include "KalmanAltitude.h"
KalmanAltitude kalmanHeight;

MPU6050 mpu;
Adafruit_BMP3XX bmp;

AllPacketData allData;
SoloData soloData;

String soloMessage = "";
String AllDados = "";
String buffer = "";

bool alavancaAcionada = false;
float initial_altitude;

#include "buzzer.h"
#include "telemetry.h"
#include "moduleSD.h"
#include "BMPMPU.h"
#include "skibs.h"
#include "GPS.h"



void SensorTask (void * parameters) {
  double end=0;
  for(;;) {
    double dt = millis()-end;
    end = millis();
    // Serial.print(dt);
    // Serial.println(" ms (task period)");

    // Medições BMP390
    if(ENABLE_BMP) {
      readBMP();
    }

    // Medições MPU6050
    if(ENABLE_MPU) {
      readMPU();
    }

    if(ENABLE_MPU) {
      kalmanHeight.update(allData.data.acceleration_Z, allData.data.altitude, dt / 1000.0);
    }

    vTaskDelay(1);
  }
  vTaskDelete( NULL );
}

void setup() {
  Wire.begin();
  Wire.setClock(400000);
  buffer.reserve(1500);
  AllDados.reserve(1500);
  // Inicializa a serial
  Serial.begin(115200);

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

  if(ENABLE_MPU) {
    setupMPU();
  }
  
  if(ENABLE_BMP) {
    setupBMP();
  }

  if(ENABLE_SKIBS) {
    setupSkibPins();
  }

  if(ENABLE_GPS) {
    setupGPS();
  }

  allData = {
    { 0,0,0,0,0,0,0 },
    { 0,0 }
  };
  soloData = { 0 };


  delay(1000);
  initial_altitude = bmp.readAltitude(1017.3);
  xTaskCreate(SensorTask, "SensorTask", 5000, NULL, 1, 0);
}

void loop() {
  if(digitalRead(ALAVANCA) == HIGH) {
    if(alavancaAcionada == false) {
      activateBuzzer();
      delay(300);
      desactivateBuzzer();
      delay(300);
      activateBuzzer();
      delay(300);
      desactivateBuzzer();
      initial_altitude = bmp.readAltitude(1017.3);
    }
    alavancaAcionada = true;

    // Armazena o tempo do microcontrolador
    allData.data.time = millis() / 1000.0;

    analyzeStateOfRocket();
    if(ENABLE_SKIBS) {
      if(isDropping) {
        if(parachute1Activated == false) {
          activateStage1();
        }
        if(parachute1Activated && millis() - timeForStage1 >= SKIB_TIME) {
          deactivateStage1();
          desactivateBuzzer();
        }

        if(parachute1Activated && parachute2Activated == false && allData.data.variationAltitude <= SECOND_APOGEE && maximumAltitudeValue >= SECOND_APOGEE) {
          activateStage2();
        }
        if(parachute2Activated && millis() - timeForStage2 >= SKIB_TIME) {
          deactivateStage2();
        }
      }
    }
    if(parachute1Activated) {
      allData.data.parachute = 1;
      if(ENABLE_BUZZER) {
        activateBuzzer();
      }
    }

    //Medições GPS
    if(ENABLE_GPS) {
      updateGPSData();
    }

    // Armazena os dados em uma string
    String dados = String(allData.data.time, 3)         
      + "," + String(allData.data.temperature, 3) 
      + "," + String(allData.data.altitude, 3)
      + "," + String(kalmanHeight.Altitude, 3)
      + "," + String(allData.data.acceleration_Z)    
      + "," + String(kalmanHeight.Velocity, 3)
      + "," + String(allData.data.pressure, 3)
      + "," + String(allData.data.parachute, 3);
    String GPSDados = String(allData.GPSData.latitude, 3)
      + "," + String(allData.GPSData.longitude, 3);
    AllDados = dados + "," + GPSDados;

    if(ENABLE_SD) {
      writeOnSD(buffer);
      //buffer.clear();
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

    Serial.println(AllDados);
    delay(500);
  }
}
