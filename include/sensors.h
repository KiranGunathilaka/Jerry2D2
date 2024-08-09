#pragma once

#include <Arduino.h>
#include <config.h>
#include <Wire.h>

#include <VL53L0X.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

class Sensors;

extern Sensors sensors;

class Sensors
{
public:

    void begin()
    {
        Wire.begin(SDA_pin, SCL_pin, 100000); // SDA , SCL

        pinMode(23, INPUT_PULLUP);
        pinMode(22, INPUT_PULLUP);
        writeByteGyro(CTRL1_XL, 0x50); // set the accel range to +- 4g
        writeByteGyro(CTRL2_G, 0x50);  // Set the Gyro range to +- 500 dps

        byte ctrl1 = readByteGyro(CTRL1_XL);
        byte ctrl2 = readByteGyro(CTRL2_G);

        // ToF address change
        pinMode(ToF_XSHUT_Left, OUTPUT);
        pinMode(ToF_XSHUT_Center, OUTPUT);
        pinMode(ToF_XSHUT_Right, OUTPUT);

        digitalWrite(ToF_XSHUT_Left, LOW);
        digitalWrite(ToF_XSHUT_Center, LOW);
        digitalWrite(ToF_XSHUT_Right, LOW);

        Serial.begin(115200);

        // changing address of 1st tof
        digitalWrite(ToF_XSHUT_Left, HIGH);
        delay(30);
        sensor1.init(true);
        delay(30);
        sensor1.setAddress(TOF_LEFT_ADD);

        // changing address of 2nd tof
        digitalWrite(ToF_XSHUT_Center, HIGH);
        delay(30);
        sensor2.init(true);
        delay(30);
        sensor2.setAddress(TOF_CENTER_ADD);

        // changing address of 3rd tof
        digitalWrite(ToF_XSHUT_Right, HIGH);
        delay(30);
        sensor3.init(true);
        delay(30);
        sensor3.setAddress(TOF_RIGH_ADD);

        sensor1.startContinuous();
        sensor2.startContinuous();
        sensor3.startContinuous();

        // Magnetometer initialization
        magDetect = mag.begin();
    }

    // VL530LX readings
    float *getToFReadings()
    {
        tofArr[0] = sensor1.readRangeSingleMillimeters();
        tofArr[1] = sensor2.readRangeSingleMillimeters();
        tofArr[2] = sensor3.readRangeSingleMillimeters();

        return tofArr;
    }

    // getting LSM6DS3 readings
    float *getGyroReadings()
    {
        gyroArr[0] = (float)readWord(OUTX_L_G) * 0.0175; // Convert to dps
        gyroArr[1] = (float)readWord(OUTX_L_G + 2) * 0.0175;
        gyroArr[2] = (float)readWord(OUTX_L_G + 4) * 0.0175;

        return gyroArr;
    }

    float *getAccelReadings()
    {

        accelArr[0] = (float)readWord(OUTX_L_XL) * 0.000122; // Convert to g
        accelArr[1] = (float)readWord(OUTX_L_XL) * 0.000122;
        accelArr[2] = (float)readWord(OUTX_L_XL) * 0.000122;

        return accelArr;
    }

    // GY-271 readings
    float *getMagReadings()
    {
        if (magDetect)
        {
            sensors_event_t event;
            mag.getEvent(&event);

            magArr[0] = event.magnetic.x;
            magArr[1] = event.magnetic.y;
            magArr[2] = event.magnetic.z;
        }

        return magArr;
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
        int16_t num = Wire.read();
        return num;
    }

    int16_t readWord(uint8_t reg)
    {
        Wire.beginTransmission(LSM6DS3_ADDRESS);
        Wire.write(reg);
        Wire.endTransmission(false);
        Wire.requestFrom(LSM6DS3_ADDRESS, 2);
        int16_t num = (Wire.read() | Wire.read() << 8);
        return num;
    }

private:
    // TOF
    VL53L0X sensor1, sensor2, sensor3;

    // Magnetometer
    Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

    bool magDetect = false;
    float tofArr[3] = {0.0, 0.0, 0.0};
    float gyroArr[3] = {0.0, 0.0, 0.0};
    float accelArr[3] = {0.0, 0.0, 0.0};
    float magArr[3] = {0.0, 0.0, 0.0};
};