/*
  MPU CACTUS
  Autores: Alan Victor e Marina Melo
  Agosto de 2021
*/

#include "MPU.h"
#include "Wire.h"

#define PI 3.1415926535897932384626433832795
#define lpCoef 0.1
#define DEG_RAD 0.0174533

void multQ(double &qa0, double &qa1, double &qa2, double &qa3, double qb0, double qb1, double qb2, double qb3);

double previous_time, xA, yA, zA;
double K_roll[2]={0, 4}, K_pitch[2]={0, 4};

float Kp = 30.0;
float Ki = 0.0;

void MPU6050::calcAccAngle() {
  accelAngleX = atan2(accelY, sqrt(accelZ * accelZ + accelX * accelX)) * 180 / PI;
  accelAngleY = atan2(accelX, sqrt(accelZ * accelZ + accelY * accelY)) * -180 / PI;
}

MPU6050::MPU6050(int _ADR, float accel) {
  CALIBRATED = false;

  ADR = _ADR;

  LAUNCH_ACCEL_IN_G = accel;
  
  LAUNCH_TIME = 0;
  GRAVITY = 0;
  dt = 0;

  qg0 = 1;
  qg1 = qg2 = qg3 = 0;

  gyroXOffset = gyroYOffset = gyroZOffset = 0.0;
  gyroAngleX = gyroAngleY = gyroAngleZ = 0.0;
}

bool MPU6050::begin(){
  Wire.begin();
  Wire.beginTransmission(ADR);
  Wire.write(0x6B);
  Wire.write(0x01);
  int result = Wire.endTransmission();
  if(result != 0)
    return false;

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
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x04);
  Wire.endTransmission();

  Wire.beginTransmission(0x68);
  Wire.write(0x19);
  Wire.write(0x00);
  Wire.endTransmission();

  return true;
}

void MPU6050::readData(){
  Wire.beginTransmission(ADR);  
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(ADR, 14, 1);

  rawAccelX=(int16_t)((int16_t)Wire.read()<<8 | Wire.read()); // g
  rawAccelY=(int16_t)((int16_t)Wire.read()<<8 | Wire.read()); // g
  rawAccelZ=(int16_t)((int16_t)Wire.read()<<8 | Wire.read()); // g
  rawTemp=(int16_t)(Wire.read()<<8|Wire.read());  // Celsus
  rawGyroX=(int16_t)(Wire.read()<<8|Wire.read()); // Dps 
  rawGyroY=(int16_t)(Wire.read()<<8|Wire.read()); // Dps 
  rawGyroZ=(int16_t)(Wire.read()<<8|Wire.read()); // Dps

  // accelX = (double)rawAccelX/2048.0;
  // accelY = (double)rawAccelY/2048.0;
  // accelZ = (double)rawAccelZ/2048.0;
  // temp = (double)rawTemp/340.0 + 36.53;
  // gyroX = (double)rawGyroX/32.8;
  // gyroY = (double)rawGyroY/32.8;
  // gyroZ = (double)rawGyroZ/32.8;
  
  // Troca de eixos
  // X para cima
  // float r = accelZ;
  // float s = gyroZ;
  // accelZ = accelX;
  // accelX = -r;
  // gyroZ = gyroX;
  // gyroX = -s;

  dt = (double)(millis()-previous_time)/1000.0;
  previous_time = (double)millis();
}

void MPU6050::update() {
  readData();

  if(CALIBRATED){

    accelX = (double)(rawAccelX - accelXOffset)/2048.0;
    accelY = (double)(rawAccelY - accelYOffset)/2048.0;
    accelZ = (double)(rawAccelZ - accelZOffset)/2048.0;
    gyroX = (double)(rawGyroX - gyroXOffset)/32.8;
    gyroY = (double)(rawGyroY - gyroYOffset)/32.8;
    gyroZ = (double)(rawGyroZ - gyroZOffset)/32.8;

    AccelFiltro();

    // getQg();
    // getQa();
    Mahony_update(accelX, accelY, accelZ, gyroX * 0.01745329, gyroY * 0.01745329, gyroZ * 0.01745329, dt);

    gravityX = 2 * (qg1*qg3 - qg0*qg2);
    gravityY = 2 * (qg0*qg1 + qg2*qg3);
    gravityZ = qg0*qg0 - qg1*qg1 - qg2*qg2 + qg3*qg3;

    //getQEuler();

    updateAccInWorldFrame();
    updateVelocity();

    if(LAUNCH_TIME == 0 && accWorldFrameZ >= LAUNCH_ACCEL_IN_G){
      LAUNCH_TIME = millis() / 1000.0;
    }
  }
}

void MPU6050::setAccelOffset(int16_t aX, int16_t aY, int16_t aZ) {
  accelXOffset = aX;
  accelYOffset = aY;
  accelZOffset = aZ;
}

void MPU6050::calibrate(int ITERATIONS){
  for(int i=0 ; i<100 ; ++i){
    readData();
  }
  for(int i=0 ; i<ITERATIONS ; ++i){
    readData();

    gyroXOffset += rawGyroX;
    gyroYOffset += rawGyroY;
    gyroZOffset += rawGyroZ;

    accelX = (double)(rawAccelX - accelXOffset)/2048.0;
    accelY = (double)(rawAccelY - accelYOffset)/2048.0;
    accelZ = (double)(rawAccelZ - accelZOffset)/2048.0;
	
    GRAVITY += sqrt(accelX*accelX + accelY*accelY + accelZ*accelZ);
  }

  CALIBRATED = true;

  gyroXOffset /= (double)ITERATIONS;
  gyroYOffset /= (double)ITERATIONS;
  gyroZOffset /= (double)ITERATIONS;

  gyroX = (double)(rawGyroX - gyroXOffset)/32.8;
  gyroY = (double)(rawGyroY - gyroYOffset)/32.8;
  gyroZ = (double)(rawGyroZ - gyroZOffset)/32.8;

  AccelFiltro();

  GRAVITY /= (double)ITERATIONS*(accelZ>0 ? +1 : -1);
}

void MPU6050::updateVelocity(){
  velX += accWorldFrameX*dt;
  velY += accWorldFrameY*dt;
  velZ += accWorldFrameZ*dt;
}

// Calcula a aceleração nas Coordenadas Globais
void MPU6050::updateAccInWorldFrame(){
  double q0=qg0, q1=qg1, q2=qg2, q3=qg3;
  double qaux0=q0, qaux1=q1, qaux2=q2, qaux3=q3;
  multQ(qaux0, qaux1, qaux2, qaux3, 0, accelX-gravityX, accelY-gravityY, accelZ-gravityZ);
  multQ(qaux0, qaux1, qaux2, qaux3, q0, -q1, -q2, -q3);

  accWorldFrameX = qaux1;
  accWorldFrameY = qaux2;
  accWorldFrameZ = qaux3;
}

void multQ(double &qa0, double &qa1, double &qa2, double &qa3, double qb0, double qb1, double qb2, double qb3) {
  double q0=qa0, q1=qa1, q2=qa2, q3=qa3;
  qa0 = (q0*qb0 - q1*qb1 - q2*qb2 - q3*qb3);
  qa1 = (q0*qb1 + q1*qb0 + q2*qb3 - q3*qb2);
  qa2 = (q0*qb2 - q1*qb3 + q2*qb0 + q3*qb1);
  qa3 = (q0*qb3 + q1*qb2 - q2*qb1 + q3*qb0);
}

void MPU6050::AccelFiltro(){
  lowPassFilter();
}

void MPU6050::lowPassFilter(){
  accelX = xA*lpCoef + accelX*(1-lpCoef);
  accelY = yA*lpCoef + accelY*(1-lpCoef);
  accelZ = zA*lpCoef + accelZ*(1-lpCoef);

  xA = accelX;
  yA = accelY;
  zA = accelZ;
}

void MPU6050::getQg() {
  double qdt0, qdt1, qdt2, qdt3;
  qdt0 = -(0.5)*(qg1*gyroX + qg2*gyroY + qg3*gyroZ);
  qdt1 = (0.5)*(qg0*gyroX + qg3*gyroY - qg2*gyroZ);
  qdt2 = (0.5)*(-qg3*gyroX + qg0*gyroY + qg1*gyroZ);
  qdt3 = (0.5)*(qg2*gyroX - qg1*gyroY + qg0*gyroZ);

  qg0 = qg0 + dt*qdt0;
  qg1 = qg1 + dt*qdt1;
  qg2 = qg2 + dt*qdt2;
  qg3 = qg3 + dt*qdt3;

  double norm = sqrt(qg0*qg0 + qg1*qg1 + qg2*qg2 + qg3*qg3);
  qg0 /= norm;
  qg1 /= norm;
  qg2 /= norm;
  qg3 /= norm;
}

void MPU6050::getQa() {
  double y = accelAngleY*0.0174533, x = accelAngleX*0.0174533, z = gyroAngleZ*0.0174533;
  qa0 = cos(z/2)*cos(y/2)*cos(x/2) - sin(z/2)*cos(y/2)*sin(x/2);
  qa1 = cos(z/2)*sin(y/2)*sin(x/2) - sin(z/2)*sin(y/2)*cos(x/2);
  qa2 = cos(z/2)*sin(y/2)*cos(x/2) + sin(z/2)*sin(y/2)*sin(x/2);
  qa3 = sin(z/2)*cos(y/2)*cos(x/2) + cos(z/2)*cos(y/2)*sin(x/2);
}

void MPU6050::getQEuler () {
  qyaw   = -atan2(2.0f * (qg1 * qg2 + qg0 * qg3), qg0 * qg0 + qg1 * qg1 - qg2 * qg2 - qg3 * qg3) * 57.29577951;   
  qpitch = asin(2.0f * (qg1 * qg3 - qg0 * qg2)) * 57.29577951;
  qroll  = atan2(2.0f * (qg0 * qg1 + qg2 * qg3), qg0 * qg0 - qg1 * qg1 - qg2 * qg2 + qg3 * qg3) * 57.29577951; 


  // // yaw: (about Z axis)
  // qyaw = atan2(2*qg1*qg2 - 2*qg0*qg3, 2*qg0*qg0 + 2*qg1*qg1 - 1);
  // // pitch: (nose up/down, about Y axis)
  // qpitch = atan2(gravityX , sqrt(gravityY*gravityY + gravityZ*gravityZ));
  // // roll: (tilt left/right, about X axis)
  // qroll = atan2(gravityY , gravityZ);
  // if (gravityZ < 0) {
  //     if(qpitch > 0) {
  //         qpitch = PI - qpitch; 
  //     } else { 
  //         qpitch = -PI - qpitch;
  //     }
  // }
}

void MPU6050::Mahony_update(double ax, double ay, double az, double gx, double gy, double gz, double deltat) {
  double recipNorm;
  double vx, vy, vz;
  double ex, ey, ez;  //error terms
  double qa, qb, qc;
  static double ix = 0.0, iy = 0.0, iz = 0.0;  //integral feedback terms
  double tmp;

  // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
  tmp = ax * ax + ay * ay + az * az;
  if (tmp > 0.0)
  {

    // Normalise accelerometer (assumed to measure the direction of gravity in body frame)
    recipNorm = 1.0 / sqrt(tmp);
    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;

    // Estimated direction of gravity in the body frame (factor of two divided out)
    vx = qg1 * qg3 - qg0 * qg2;
    vy = qg0 * qg1 + qg2 * qg3;
    vz = qg0 * qg0 - 0.5f + qg3 * qg3;

    // Error is cross product between estimated and measured direction of gravity in body frame
    // (half the actual magnitude)
    ex = (ay * vz - az * vy);
    ey = (az * vx - ax * vz);
    ez = (ax * vy - ay * vx);

    // Compute and apply to gyro term the integral feedback, if enabled
    if (Ki > 0.0f) {
      ix += Ki * ex * deltat;  // integral error scaled by Ki
      iy += Ki * ey * deltat;
      iz += Ki * ez * deltat;
      gx += ix;  // apply integral feedback
      gy += iy;
      gz += iz;
    }

    // Apply proportional feedback to gyro term
    gx += Kp * ex;
    gy += Kp * ey;
    gz += Kp * ez;
  }

  // Integrate rate of change of quaternion, q cross gyro term
  deltat = 0.5 * deltat;
  gx *= deltat;   // pre-multiply common factors
  gy *= deltat;
  gz *= deltat;
  qa = qg0;
  qb = qg1;
  qc = qg2;
  qg0 += (-qb * gx - qc * gy - qg3 * gz);
  qg1 += (qa * gx + qc * gz - qg3 * gy);
  qg2 += (qa * gy - qb * gz + qg3 * gx);
  qg3 += (qa * gz + qb * gy - qc * gx);

  // renormalise quaternion
  recipNorm = 1.0 / sqrt(qg0 * qg0 + qg1 * qg1 + qg2 * qg2 + qg3 * qg3);
  qg0 *= recipNorm;
  qg1 *= recipNorm;
  qg2 *= recipNorm;
  qg3 *= recipNorm;
}
