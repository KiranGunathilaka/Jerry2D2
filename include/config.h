// config.h
//change this to robot configurations. 
// we can have a nother maze config file also

#pragma once


//**************************************************ENCODER CONFIG**************************************************************************
const int LeftEncoderPin1 = 33;
const int LeftEncoderPin2 = 32;

const int RightEncoderPin1 = 35;
const int RightEncoderPin2 = 34;  // Assuming you might have another pin for Right Encoder


//**************************************************SENSOR CONFIG**************************************************************************
const int SCL_pin = 22;
const int SDA_pin = 23;

const int ToF_XSHUT_Right =5;
const int ToF_XSHUT_Center =18;
const int ToF_XSHUT_Left = 19;

#define LSM6DS3_ADDRESS 0x6B // I2C address of LSM6DS3

//IMU register adddresses
#define CTRL1_XL 0x10  // Accelerometer control register
#define CTRL2_G 0x11   // Gyroscope control register
#define OUTX_L_G 0x22  // First gyro data register
#define OUTX_L_XL 0x28 // First accel data register

//custom i2c addresses
#define TOF_RIGHT_ADD 0x30
#define TOF_CENTER_ADD 0x31
#define TOF_LEFT_ADD 0x32

const float g = 9.80665; //ms^-1
const float gyroSensitivity = 0.0175; //for dps
const float accelSensitivity = 0.000122; //for g
const float accelScaleFact = 0.0011964; // g*accelSensitivity

const float radToDeg = 57.296;
const float scaleY = 43.44; // these are the primary axis lengths of the ellipse that got from plotting x y magnetic field intensity values
const float scaleX = 46.44;  //for more detyails see the calibaration code on the Arduino codes folder
const float centerOffsetX = 20.91;
const float centerOffsetY = -2.08;

//**************************************************MOUSE CONFIG**************************************************************************
const int wheelDiameter = 32; //in mm
const int MOUSE_RADIUS = 14;
const int RADIANS_PER_DEGREE = 3.14/180;
const float WHEEL_GAP = 108; // distance between the wheels in mm

const float MM_PER_ROTATION = 103.67; //  pi*wheel diameter .......d=33mm
const float DEG_PER_MM_DIFFERENCE = 180.0/(3.14*WHEEL_GAP);

//*****************************************************COMMUNICATIONS************************************************************************
const char* SSID ="Chocolate";
const char* WIFI_PASSWORD = "Pissu69pusa";
const char* HOST_IP = "192.168.8.131"; //IP of host computer
const int HOST_PORT = 1234;  // Port number on the PC
const bool WIFI_ENABLE = true;

//***************************************************MOTOR CONFIG***************************************************************************/
//Left and Right Motor configurations
const float MAX_MOTOR_PERCENTAGE = 90;
const int MIN_MOTOR_BIAS = 10;
const int PWM_RESOLUTION_BITS = 8;
const int PWM_RESOLUTION = 256; //2^8 use a suitable code to automate this
const int PULSES_PER_ROTATION = 1430;

const float MOTOR_BALANCE = 0;    //The Percentage fed into the left(add) and right(deduct) motors to math the motor performance 
const int M_BALNCE_PWM = MAX_MOTOR_PERCENTAGE*PWM_RESOLUTION*MOTOR_BALANCE/10000;


const int LEFT_MOTOR_PWM = 25;   //left is motor A
const int LEFT_MOTOR_IN1 = 26;
const int LEFT_MOTOR_IN2 = 27;
const int RIGHT_MOTOR_IN2 = 14; //right is motor B
const int RIGHT_MOTOR_IN1 = 12;
const int RIGHT_MOTOR_PWM = 13;


//change motor directions to make the motors spin forward when both motors are forward commanded
#define MOTOR_LEFT_POLARITY (-1)
#define MOTOR_RIGHT_POLARITY (1)

//PD parameters   //make temporaly non const
 float FWD_KP = 0.05; 
const float FWD_KD = 0.01;
const float ROT_KP = 1;
const float ROT_KD = 1;

//**************************************************REPORTING CONFIG**************************************************************************
uint8_t broadcastAddress[] = { 0xEC, 0xDA, 0x3B, 0x51, 0xA5, 0x84 }; // RECEIVER MAC Address