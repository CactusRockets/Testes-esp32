/*
  MPU CACTUS
  Autores: Alan Victor e Marina Melo
  Agosto de 2021
*/

#ifndef mpu_H
#define mpu_H
#include "Arduino.h"

class MPU6050
{
	public:
	  MPU6050(int _ADR = 0x68, float _CONST = 0.02, float accel=10);
		int begin();
		void calibrate(int ITERATIONS = 3000);
	    
		void readData();
		void update();
		void updateVelocity();
		void updateAccInWorldFrame();

		void AccelFiltro();
		void lowPassFilter();

    void calcAccAngle();

    double dt;
    double LAUNCH_TIME, LAUNCH_ACCEL_IN_G;

	  int16_t rawAccelX, rawAccelY, rawAccelZ;
    int16_t rawGyroX, rawGyroY, rawGyroZ, rawTemp;
    double accelX, accelY, accelZ;
		double gyroX, gyroY, gyroZ, temp;
		double gyroXOffset, gyroYOffset, gyroZOffset;

		double gyroAngleX, gyroAngleY, gyroAngleZ;
		double accelAngleX, accelAngleY;

		double yaw, pitch, roll;
		double accWorldFrameX, accWorldFrameY, accWorldFrameZ;
		double velX, velY, velZ;
		
		double GRAVITY;
   
	private:
		bool CALIBRATED;
		double CONST;
		int ADR;
};

#endif
