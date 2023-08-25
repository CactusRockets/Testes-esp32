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
	  MPU6050(int _ADR = 0x68, float accel=10);
		bool begin();
		void calibrate(int ITERATIONS = 3000);
    void setAccelOffset(int16_t aX, int16_t aY, int16_t aZ);
	    
		void readData();
		void update();
		void updateVelocity();
		void updateAccInWorldFrame();

		void AccelFiltro();
		void lowPassFilter();

    void calcAccAngle();

    void getQg();
    void getQa();
    void getQEuler();
    void Mahony_update(double ax, double ay, double az, double gx, double gy, double gz, double deltat);

    double dt;
    double LAUNCH_TIME, LAUNCH_ACCEL_IN_G;

	  int16_t rawAccelX, rawAccelY, rawAccelZ;
    int16_t rawGyroX, rawGyroY, rawGyroZ, rawTemp;
    double accelX, accelY, accelZ;
		double gyroX, gyroY, gyroZ, temp;
		int16_t gyroXOffset, gyroYOffset, gyroZOffset;
		int16_t accelXOffset, accelYOffset, accelZOffset;

    double qg0, qg1, qg2, qg3;
    double qa0, qa1, qa2, qa3;
    double qroll, qpitch, qyaw;

    double gravityX, gravityY, gravityZ;

		double gyroAngleX, gyroAngleY, gyroAngleZ;
		double accelAngleX, accelAngleY;

		double yaw, pitch, roll;
		double accWorldFrameX, accWorldFrameY, accWorldFrameZ;
		double velX, velY, velZ;
		
		double GRAVITY;
	private:
		bool CALIBRATED;
		int ADR;
		
};

#endif

