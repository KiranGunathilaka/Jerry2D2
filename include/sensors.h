#pragma once

#include <Arduino.h>
#include <config.h>

#include <VL53L0X.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#define LSM6DS3_ADDRESS 0x6B // I2C address of LSM6DS3

#define CTRL1_XL 0x10  // Accelerometer control register
#define CTRL2_G 0x11   // Gyroscope control register
#define OUTX_L_G 0x22  // First gyro data register
#define OUTX_L_XL 0x28 // First accel data register

class Sensors;

extern Sensors sensors;

class Sensors
{
public:

    void begin()
    {
        pinMode(23, INPUT_PULLUP);
        pinMode(22, INPUT_PULLUP);
        writeByteGyro(CTRL1_XL, 0x54); // set the accel range to +- 4g
        writeByteGyro(CTRL2_G, 0x54);  // Set the Gyro range to +- 500 dps

        byte ctrl1 = readByteGyro(CTRL1_XL);
        byte ctrl2 = readByteGyro(CTRL2_G);

        // ToF address change
        pinMode(ToF_XSHUT_Left, OUTPUT);
        pinMode(ToF_XSHUT_Center, OUTPUT);
        pinMode(ToF_XSHUT_Right, OUTPUT);

        digitalWrite(ToF_XSHUT_Left, LOW);
        digitalWrite(ToF_XSHUT_Center, LOW);
        digitalWrite(ToF_XSHUT_Right, LOW);

        Wire.begin(SDA_pin, SCL_pin); // SDA , SCL
        Wire.setClock(100000);

        // changing address of 1st tof
        digitalWrite(5, HIGH);
        delay(30);
        sensor1.init(true);
        Serial.println("01");
        delay(30);
        sensor1.setAddress(TOF_LEFT_ADD);

        // changing address of 2nd tof
        digitalWrite(18, HIGH);
        delay(30);
        sensor2.init(true);
        Serial.println("02");
        delay(30);
        sensor2.setAddress(TOF_CENTER_ADD);

        // changing address of 3rd tof
        digitalWrite(19, HIGH);
        delay(30);
        sensor3.init(true);
        Serial.println("03");
        delay(30);
        sensor3.setAddress(TOF_RIGH_ADD);

        Serial.println("ToF addresses set");

        sensor1.startContinuous();
        sensor2.startContinuous();
        sensor3.startContinuous();

        // Magnetometer initialization
        magDetect = mag.begin();
    }

    void writeByteGyro(uint8_t reg, uint8_t value)
    {
        Wire.beginTransmission(LSM6DS3_ADDRESS);
        Wire.write(reg);
        Wire.write(value);
        Wire.endTransmission();
    }

    int16_t readByteGyro(uint8_t reg)
    {
        Wire.beginTransmission(LSM6DS3_ADDRESS);
        Wire.write(reg);
        Wire.endTransmission(false);
        Wire.requestFrom(LSM6DS3_ADDRESS, 1);
        return Wire.read();
    }

    int16_t readWord(uint8_t reg)
    {
        Wire.beginTransmission(LSM6DS3_ADDRESS);
        Wire.write(reg);
        Wire.endTransmission(false);
        Wire.requestFrom(LSM6DS3_ADDRESS, 2);
        return (Wire.read() | Wire.read() << 8);
    }

private:
    // TOF
    VL53L0X sensor1, sensor2, sensor3;
    int a, b, c, d;

    // Magnetometer
    Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

    bool magDetect = false;
};