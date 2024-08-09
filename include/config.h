// config.h
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


//custom i2c addresses

#define TOF_LEFT_ADD 0x30
#define TOF_CENTER_ADD 0x31
#define TOF_RIGH_ADD 0x32


const int wheelDiameter = 32; //in mm
const int gearRatio = 20; //find this

const int MM_PER_COUNT_LEFT = 20; 
const int MM_PER_COUNT_RIGHT = 20;
const int DEG_PER_MM_DIFFERENCE = 0.76;