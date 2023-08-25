/* CONFIGURAÇÕES BMP e MPU */
#define BMP_ADRESS 0x77
#define MPU_ADRESS 0x68

float velocidadeAtual = 0;

float maximumAltitudeValue = 0;
float altitudeAtual = 0;

void setupBMP() {
  if(!bmp.begin_I2C(BMP_ADRESS)) {
    Serial.println("Failed to find BMP390 chip");
    while(1);
  }
  Serial.println("BMP conectado!");

  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_6_25_HZ);

  delay(200);
  if (!bmp.performReading()) {
    Serial.println("Failed to read");
  }
}

void readBMP() {
  if (!bmp.performReading()) {
    //Serial.println("Failed to read");
    return;
  }

  allData.data.temperature = bmp.temperature;
  allData.data.pressure = bmp.pressure;
  allData.data.altitude = bmp.readAltitude(1017.3);
  
  // altitudeAtual = allData.data.altitude;
  altitudeAtual = kalmanHeight.Altitude;

  if(altitudeAtual > maximumAltitudeValue) {
    maximumAltitudeValue = altitudeAtual;
  }
}

void setupMPU() {
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while(1);
  }
  Serial.println("MPU6050 conectado!");

  Serial.println("Calibrando...");
  mpu.setAccelOffset(5708, 4836, 10574);
  mpu.calibrate();
  Serial.println("Calibrado!");
}

void readMPU() {
  mpu.update();

  allData.data.acceleration_Z =  mpu.accWorldFrameZ * 9.80665; // Transform g to m/s^2
  velocidadeAtual = mpu.velZ * 9.80665;
}
