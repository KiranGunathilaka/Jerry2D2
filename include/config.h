// config.h
//change this to robot configurations. 
// we can have a nother maze config file also

#pragma once
#include "esp_system.h"


const int loopTime = 18500; //in micro seconds

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

const float RIGHT_DISTANCE_THRESHOLD = 95.0; //in mm
const float LEFT_DISTANCE_THRESHOLD = 95.0; //in mm
const float FRONT_THRESHOLD = 100.0; //in mm
const float SIDE_DISTANCE = 55.0;

const float STEERING_KP = 0.1;
const float STEERING_KD = 1.5;
const float STEERING_ADJUST_LIMIT = 10.0;

const float rightFrontSideAngle = 38.6;
const float leftFrontSideAngle = 47.0;
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
const int MIN_MOTOR_PERCENTAGE = 5; // when the given percentage is below this value, percentage is set to zero to damp oscillations
const int MIN_MOTOR_BIAS = 10;// miinimum percentage that should be given for the motors to spin
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

//PD parameters  
const float FWD_KP_ = 0.01;
const float FWD_KD = 0.64;
const float ROT_KP = 1.3;
const float ROT_KD = 1.2;

const float FWD_KP_SMALL = 0.5;    
const float FWD_KD_SMALL = 0.9;

const float ROT_KP_90 = 2.4;   // measured for(90,360,0,3600)   @7.4V battery
const float ROT_KD_90 = 3;

//**************************************************REPORTING CONFIG**************************************************************************
uint8_t broadcastAddress[] = { 0xEC, 0xDA, 0x3B, 0x51, 0xA5, 0x84 }; // RECEIVER MAC Address

//**************************************************MOUSE CONFIG****************************************************************************

const int FULL_CELL = 180; //in mm
const int HALF_CELL = 90;
const int INITIAL_OFFSET_FROM_CENTER = 0;
const int STOP_DISTANCE = 60; // sum of the distance readings when mouse positioned centered in a cell before a front wall
const int FORWARD_STOP_POSITION_FROM_WALL = 45;

const int OMEGA_SPIN_TURN = 360;
const int ALPHA_SPIN_TURN = 3600;

const int SEARCH_SPEED= 320;
const int SEARCH_ACCELERATION = 1000;

const int EXTRA_WALL_ADJUST = 12; //wall thickness

const int ERROR_CORRECTION_OMEGA = 100;
const int ERROR_CORRECTION_ALPHA = 1000;

const int BACK_WALL_TO_CENTER = 0; //distance that need to be travelled to go to the center when robot is against the backwall

//***************************************************Switches******************************************************************************
const int SWITCH_PIN = 15;
const int BATTERY_PIN = 4;

//***************************************************Maze*******************************************************************************


#define TARGET1 Location(3,4)
#define TARGET2 Location(3,4)
#define TARGET3 Location(3,4)
#define TARGET4 Location(3,4)
#define MAZE_WIDTH 8
#define MAZE_HEIGHT 8
#define MAZE_CELL_COUNT (MAZE_WIDTH * MAZE_HEIGHT)
#define MAX_COST (MAZE_CELL_COUNT - 1)

#define STACK_SIZE 256