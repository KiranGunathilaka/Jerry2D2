// config.h
//change this to robot configurations. 
// we can have a nother maze config file also

#pragma once
#include "esp_system.h"


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
//const int ToF_XSHUT_CenLeft = 21;
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
//#define TOF_CENLEFT_ADD 0x32
#define TOF_LEFT_ADD 0x33

const float g = 9.80665; //ms^-1
const float gyroSensitivity = 0.0175; //for dps
const float accelSensitivity = 0.000122; //for g
const float accelScaleFact = 0.0011964; // g*accelSensitivity

const float radToDeg = 57.296;
const float scaleY = 43.44; // these are the primary axis lengths of the ellipse that got from plotting x y magnetic field intensity values
const float scaleX = 46.44;  //for more detyails see the calibaration code on the Arduino codes folder
const float centerOffsetX = 20.91;
const float centerOffsetY = -2.08;

const float RIGHT_DISTANCE_THRESHOLD = 80.0; //in mm
const float LEFT_DISTANCE_THRESHOLD = 80.0; //in mm
const float FRONT_THRESHOLD = 100.0; //in mm
const float SIDE_DISTANCE = 55.0;

const float STEERING_KP = 0.8;
const float STEERING_KD = 1.3;
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
const int MIN_MOTOR_PERCENTAGE = 15; // when the given percentage is below this value, percentage is set to zero to damp oscillations
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
const float FWD_KP = 0.01;
const float FWD_KD = 0.64;
const float ROT_KP = 1.3;
const float ROT_KD = 1.2;

//**************************************************REPORTING CONFIG**************************************************************************
uint8_t broadcastAddress[] = { 0xEC, 0xDA, 0x3B, 0x51, 0xA5, 0x84 }; // RECEIVER MAC Address

//**************************************************MOUSE CONFIG****************************************************************************

const int FULL_CELL = 180; //in mm
const int HALF_CELL = 90;
const int FRONT_REFERENCE = 200; // sum of the distance readings when mouse positioned centered in a cell before a front wall

const int OMEGA_SPIN_TURN = 360;
const int ALPHA_SPIN_TURN = 3600;

const int SEARCH_TURN_SPEED= 300;

const int EXTRA_WALL_ADJUST = 12; //wall thickness

//the position where the sensors are supposed to get readings if IR sensors were used
//need to update what do to when using ToF
const float SENSING_POSITION = 170.0;

const int SEARCH_SPEED = 300;
const int SEARCH_ACCELERATION = 1000;

const int BACK_WALL_TO_CENTER = 0; //distance that need to be travelled to go to the center when robot is against the backwall

struct TurnParameters {
  int speed;         // mm/s    - constant forward speed during turn
  int entry_offset;  // mm      - distance from turn pivot to turn start
  int exit_offset;   // mm      - distance from turn pivot to turn end
  float angle;       // deg     - total turn angle
  float omega;       // deg/s   - maximum angular velocity
  float alpha;       // deg/s/s - angular acceleration
  int trigger;       //         - front sensor value at start of turn
};

// const TurnParameters turn_params[4] = {
//     //           speed, entry,   exit, angle, omega,  alpha, sensor threshold
//     {SEARCH_TURN_SPEED,    70,     80,  90.0, 287.0, 2866.0, TURN_THRESHOLD_SS90E}, // 0 => SS90EL
//     {SEARCH_TURN_SPEED,    70,     80, -90.0, 287.0, 2866.0, TURN_THRESHOLD_SS90E}, // 0 => SS90ER
//     {SEARCH_TURN_SPEED,    70,     80,  90.0, 287.0, 2866.0, TURN_THRESHOLD_SS90E}, // 0 => SS90L
//     {SEARCH_TURN_SPEED,    70,     80, -90.0, 287.0, 2866.0, TURN_THRESHOLD_SS90E}, // 0 => SS90R
// };