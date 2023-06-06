/*
  MPU CACTUS
  Autores: Alan Victor e Marina Melo
  Agosto de 2021
*/

#include "MPU.h"
#include "Wire.h"
#include "Kalman.h"

Kalman kalmanRool;
Kalman kalmanPitch;

#define PI 3.1415926535897932384626433832795
#define lpCoef 0.05
#define DEG_TO_RAD 0.0174533

double previous_time; // Variavel auxiliar para calcular o clock de leitura
double xA, yA, zA;    // Variáveis para Low Pass Filter

void MPU6050::calcAccAngle() {
  // Get Roll (angleX) and Pitch (angleY) by accelerometer
  accelAngleX = atan2(accelY, sqrt(accelZ * accelZ + accelX * accelX)) * 180 / PI;
  accelAngleY = atan2(accelX, sqrt(accelZ * accelZ + accelY * accelY)) * -180 / PI;
}

MPU6050::MPU6050(int _ADR, float _CONST, float accel)
{
  CALIBRATED = false;

  ADR = _ADR;       // Endereço I2C do MPU
  CONST = _CONST;   // Constante para filtro complementar

  LAUNCH_ACCEL_IN_G = accel; // Aceleracao esperada para deteccao de lancamento
  
  // Reset de algumas variaveis
  LAUNCH_TIME = 0;
  GRAVITY = 0;      // Variavel que vai guardar o calculo da gravidade na calibracao
  dt = 0;           // Clock de leitura

  gyroXOffset = gyroYOffset = gyroZOffset = 0.0;
  gyroAngleX = gyroAngleY = gyroAngleZ = 0.0;
}

int MPU6050::begin(){
  // Inicia MPU
  Wire.begin();
  int result = Wire.beginTransmission(ADR);
  Wire.write(0x6B);
  Wire.write(0x01);
  Wire.endTransmission();
  if(result != 0)
    return 0;

  /*
  Configura o giroscópio

  Wire.write(0b00000000); //   +/- 250 °/s
  Wire.write(0b00001000); //   +/- 500 °/s
  Wire.write(0b00010000); //   +/- 1000 °/s
  Wire.write(0b00011000); //   +/- 2000 °/s
  */

  Wire.beginTransmission(ADR);
  Wire.write(0x1B);
  Wire.write(0b00010000);
  Wire.endTransmission();
  if(result != 0)
    return 0;
 
  /*
  Configura o acelerômetro

  Wire.write(0b00000000); //   +/- 2g
  Wire.write(0b00001000); //   +/- 4g
  Wire.write(0b00010000); //   +/- 8g
  Wire.write(0b00011000); //   +/- 16g
  */

  Wire.beginTransmission(ADR);
  Wire.write(0x1C);
  Wire.write(0b00011000);
  Wire.endTransmission();


  // Outras
  // DLPF Config
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x04);
  Wire.endTransmission();

  // SMPLRT Config
  Wire.beginTransmission(0x68);
  Wire.write(0x19);
  Wire.write(0x00);
  Wire.endTransmission();

  return 1;
}

void MPU6050::readData(){
  Wire.beginTransmission(ADR);  
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(ADR, 14, 1);

  rawAccelX=(int16_t)((int16_t)Wire.read()<<8 | Wire.read()); // g
  rawAccelY=(int16_t)((int16_t)Wire.read()<<8 | Wire.read()); // g
  rawAccelZ=(int16_t)((int16_t)Wire.read()<<8 | Wire.read()); // g
  rawTemp=(int16_t)(Wire.read()<<8|Wire.read());  // Celsius
  rawGyroX=(int16_t)(Wire.read()<<8|Wire.read()); // Dps 
  rawGyroY=(int16_t)(Wire.read()<<8|Wire.read()); // Dps 
  rawGyroZ=(int16_t)(Wire.read()<<8|Wire.read()); // Dps

  accelX = (double)rawAccelX/2048.0;
  accelY = (double)rawAccelY/2048.0;
  accelZ = (double)rawAccelZ/2048.0;
  temp = (double)rawTemp/340.0 + 36.53;
  gyroX = (double)rawGyroX/32.8;
  gyroY = (double)rawGyroY/32.8;
  gyroZ = (double)rawGyroZ/32.8;
  
  // Troca de eixos
  // Y para cima
  /*
  float r = accelZ;
  float s = gyroZ;
  accelZ = accelY;
  accelY = -r;
  gyroZ = gyroY;
  gyroY = -s;
  */

  // Calculo do clock
  dt = (double)(millis()-previous_time)/1000.0;
  previous_time = (double)millis();
}

void MPU6050::update()
{
  readData();

  if(CALIBRATED){
    AccelFiltro();  // Submete o acelerometro a um Low Pass Filter em codigo
    calcAccAngle(); // Pega os calculos de angulo com base na aceleracao

    // Calibra as leituras do giroscopio
    gyroX -= gyroXOffset;
    gyroY -= gyroYOffset;
    gyroZ -= gyroZOffset;

    // Integra as taxas de giro
    gyroAngleX += gyroX*dt;
    gyroAngleY += gyroY*dt;
    gyroAngleZ += gyroZ*dt;

    // Utiliza um filtro de kalman 1d para pegar o pitch e roll
    roll = kalmanRool.getAngle(accelAngleX, gyroX, dt);
    pitch = kalmanPitch.getAngle(accelAngleY, gyroY, dt);
    yaw = gyroAngleZ;

    updateAccInWorldFrame(); // Calcula aceleracoes no sistema inercial
    updateVelocity();        // Calcula as velocidades

    // Deteccao de Lancamento
    if(LAUNCH_TIME == 0 && accWorldFrameZ >= LAUNCH_ACCEL_IN_G){
      LAUNCH_TIME = millis() / 1000.0;
    }
  }
}

void MPU6050::calibrate(int ITERATIONS){
  for(int i=0 ; i<100 ; ++i){
    readData();
  }
  for(int i=0 ; i<ITERATIONS ; ++i){
    readData();

    gyroXOffset += gyroX;
    gyroYOffset += gyroY;
    gyroZOffset += gyroZ;
	
    GRAVITY += sqrt(accelX*accelX + accelY*accelY + accelZ*accelZ);
  }

  CALIBRATED = true;

  gyroXOffset /= (double)ITERATIONS;
  gyroYOffset /= (double)ITERATIONS;
  gyroZOffset /= (double)ITERATIONS;

  gyroX -= gyroXOffset;
  gyroY -= gyroYOffset;
  gyroZ -= gyroZOffset;

  AccelFiltro();
  calcAccAngle();

  roll = accelAngleX;
  pitch = accelAngleY;

  gyroAngleX = roll;
  gyroAngleY = pitch;

  // Seta os valores iniciais
  kalmanRool.setAngle(roll);
  kalmanPitch.setAngle(pitch);

  GRAVITY /= (double)ITERATIONS;
}

void MPU6050::updateVelocity(){
  velX += accWorldFrameX*dt;
  velY += accWorldFrameY*dt;
  velZ += accWorldFrameZ*dt;
}

// Calcula a aceleração nas Coordenadas Globais
void MPU6050::updateAccInWorldFrame(){
  double x = roll*0.0174533;
  double y = pitch*0.0174533;
  double z = yaw*0.0174533;

  accWorldFrameX = ( accelX )*( cos(z)*cos(y) )     + ( accelY )*( cos(z)*sin(y)*sin(x) - sin(z)*cos(x) ) + ( accelZ )*( cos(z)*sin(y)*cos(x) + sin(z)*sin(x) );
  accWorldFrameY = ( accelX )*( sin(z)*cos(y) )     + ( accelY )*( sin(z)*sin(y)*sin(x) + cos(z)*cos(x) ) + ( accelZ )*( sin(z)*sin(y)*cos(x) - cos(z)*sin(x) ); 
  accWorldFrameZ = ((accelZ < 0 ? 1 : -1)*GRAVITY) + ( accelX )*( -sin(y) ) + ( accelY )*( cos(y)*sin(x) )                     + ( accelZ )*( cos(y)*cos(x) );
}

void MPU6050::AccelFiltro(){
  //lowPassFilter();
}

void MPU6050::lowPassFilter(){
  accelX = xA*lpCoef + accelX*(1-lpCoef);
  accelY = yA*lpCoef + accelY*(1-lpCoef);
  accelZ = zA*lpCoef + accelZ*(1-lpCoef);

  xA = accelX;
  yA = accelY;
  zA = accelZ;
}

