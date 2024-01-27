/* CONFIGURAÇÕES BMP e MPU */

#define BMP_ADRESS 0x76
#define MPU_ADRESS 0x68

Adafruit_MPU6050 mpu;
Adafruit_BMP280 bmp;

float velocidadeAtual = 0;

float maximumAltitudeValue = 0;
float altitudeAtual = 0;

void setupBMP() {
  if(!bmp.begin(BMP_ADRESS)) {
    Serial.println("Failed to find BMP280 chip");
    while(1);
  }
  Serial.println("BMP conectado!");

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X4,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X8,     /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_1);   /* Standby time. */
}

void readBMP() {
  allData.data.temperature = bmp.readTemperature();
  allData.data.pressure = bmp.readPressure();
  allData.data.altitude = bmp.readAltitude(1017.3);
  allData.data.variation_altitude = bmp.readAltitude(1017.3) - initial_altitude;
  
  altitudeAtual = allData.data.altitude;

  if(altitudeAtual > maximumAltitudeValue) {
    maximumAltitudeValue = altitudeAtual;
  }
}

void setupMPU() {
  if (!mpu.begin(MPU_ADRESS)) {
    Serial.println("Failed to find MPU6050 chip");
    while(1);
  }
  Serial.println("MPU6050 conectado!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void readMPU() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  allData.data.acceleration_Z = a.acceleration.z;

  allData.data.altitude_MPU = 0;
  velocidadeAtual = 0;
}
