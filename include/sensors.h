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
        int16_t gx = readWord(OUTX_L_G);
        int16_t gy = readWord(OUTX_L_G + 2);
        int16_t gz = readWord(OUTX_L_G + 4);

        gyroArr[0] = gx * 0.0175f; // Convert to dps
        gyroArr[1] = gy * 0.0175f;
        gyroArr[2] = gz * 0.0175f;

        return gyroArr;
    }

    float *getAccelReadings()
    {
        int16_t ax = readWord(OUTX_L_XL);
        int16_t ay = readWord(OUTX_L_XL + 2);
        int16_t az = readWord(OUTX_L_XL + 4);

        accelArr[0] = ax * 0.122f / 1000.0f; // Convert to g
        accelArr[1] = ay * 0.122f / 1000.0f;
        accelArr[2] = az * 0.122f / 1000.0f;

        return accelArr;
    }

    // GY-271 readings
    float *getMagReadings()
    {
        if (magDetect) {
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
    float tofArr[3] = {0.0, 0.0, 0.0};
    float gyroArr[3] = {0.0, 0.0, 0.0};
    float accelArr[3] = {0.0, 0.0, 0.0};
    float magArr[3] = {0.0, 0.0, 0.0};
};