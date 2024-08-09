// config.h
//change this to robot configurations. 
// we can have a nother maze config file also

#pragma once


//pins
const int LeftEncoderPin1 = 33;
const int LeftEncoderPin2 = 32;

const int RightEncoderPin1 = 35;
const int RightEncoderPin2 = 34;  // Assuming you might have another pin for Right Encoder

const int SCL_pin = 22;
const int SDA_pin = 23;

const int ToF_XSHUT_Left =5;
const int ToF_XSHUT_Center =18;
const int ToF_XSHUT_Right = 19;



#define LSM6DS3_ADDRESS 0x6B // I2C address of LSM6DS3

#define CTRL1_XL 0x10  // Accelerometer control register
#define CTRL2_G 0x11   // Gyroscope control register
#define OUTX_L_G 0x22  // First gyro data register
#define OUTX_L_XL 0x28 // First accel data register
//custom i2c addresses

#define TOF_LEFT_ADD 0x30
#define TOF_CENTER_ADD 0x31
#define TOF_RIGH_ADD 0x32


const int wheelDiameter = 32; //in mm
const int MOUSE_RADIUS = 14;
const int RADIANS_PER_DEGREE = 3.14/180;
const float WHEEL_GAP = 108; // distance between the wheels in mm

const float MM_PER_ROTATION = 103.67; //  pi*wheel diameter .......d=33mm
const int DEG_PER_MM_DIFFERENCE = 180/(3.14*WHEEL_GAP);

//*********MOTOR CONFIG************************************************** */
//Left and Right Motor configurations
const float MAX_MOTOR_PERCENTAGE = 100;
const int MIN_MOTOR_BIAS = 1;
const int PWM_RESOLUTION_BITS = 8;
const int PWM_RESOLUTION = 256; //2^8 use a suitable code to automate this
const int PULSES_PER_ROTATION = 1430;

const float MOTOR_BALANCE = 5;    //The Percentage fed into the left(add) and right(deduct) motors to math the motor performance 
const int M_BALNCE_PWM = MAX_MOTOR_PERCENTAGE*PWM_RESOLUTION*MOTOR_BALANCE/100;


const int LEFT_MOTOR_PWM = 25;   //left is motor A
const int LEFT_MOTOR_IN1 = 26;
const int LEFT_MOTOR_IN2 = 27;
const int RIGHT_MOTOR_IN2 = 14; //right is motor B
const int RIGHT_MOTOR_IN1 = 12;
const int RIGHT_MOTOR_PWM = 13;


//change motor directions to make the motors spin forward when both motors are forward commanded
#define MOTOR_LEFT_POLARITY (-1)
#define MOTOR_RIGHT_POLARITY (1)