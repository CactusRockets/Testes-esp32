/* BMP e MPU */
Adafruit_MPU6050 mpu;
Adafruit_BMP280 bmp;

float velocidadeAtual = 0;
float maximumAltitudeValue = 0;
float altitudeAtual = 0;

void setupBMP() {
  if(!bmp.begin(BMP_ADRESS)) {
    Serial.println("BMP not working ...");
    while(1);
  }
  Serial.println("BMP conectado!");

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X4,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X8,     /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_63);  /* Standby time. */
}

void testBMP() {
  data.temp = bmp.readTemperature();
  data.pres = bmp.readPressure();
  data.alt = bmp.readAltitude(1017.3);
  data.vaalt = bmp.readAltitude(1017.3) - altini;
}

void setupMPU() {
  if (!mpu.begin(MPU_ADRESS)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(100);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void testMPU() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  data.acz= a.acceleration.z; 
}
