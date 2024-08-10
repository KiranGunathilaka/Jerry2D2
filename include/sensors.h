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
        pinMode(ToF_XSHUT_Right, OUTPUT);
        pinMode(ToF_XSHUT_Center, OUTPUT);
        pinMode(ToF_XSHUT_Left, OUTPUT);

        digitalWrite(ToF_XSHUT_Right, LOW);
        digitalWrite(ToF_XSHUT_Center, LOW);
        digitalWrite(ToF_XSHUT_Left, LOW);

        Serial.begin(115200);

        // changing address of 1st tof
        digitalWrite(ToF_XSHUT_Right, HIGH);
        delay(30);
        tofRight.init(true);
        delay(30);
        tofRight.setAddress(TOF_RIGHT_ADD);

        // changing address of 2nd tof
        digitalWrite(ToF_XSHUT_Center, HIGH);
        delay(30);
        tofCenter.init(true);
        delay(30);
        tofCenter.setAddress(TOF_CENTER_ADD);

        // changing address of 3rd tof
        digitalWrite(ToF_XSHUT_Left, HIGH);
        delay(30);
        tofLeft.init(true);
        delay(30);
        tofLeft.setAddress(TOF_LEFT_ADD);

        tofRight.startContinuous();
        tofCenter.startContinuous();
        tofLeft.startContinuous();

        // Magnetometer initialization
        magDetect = mag.begin();
    }

    void update()
    {
        // tof update
        tofReadings[0] = tofRight.readRangeContinuousMillimeters();
        tofReadings[1] = tofCenter.readRangeContinuousMillimeters();
        tofReadings[2] = tofLeft.readRangeContinuousMillimeters();

        for (int j = 0; j < 3; j++)
        {
            sum[j] -= tofArr[j][i];
            tofArr[j][i] = tofReadings[j];
            sum[j] += tofArr[j][i];

            tofAverage[j] = (float)sum[j] / 10 + tofOffset[j];
        }

        i++;
        if (i >= 10)
        {
            i = i % 10;
        }

        // gyro update
        gyroArr[0] = (float)readWord(OUTX_L_G) * gyroSensitivity + gyroNoise[0]; // Convert to dps
        gyroArr[1] = (float)readWord(OUTX_L_G + 2) * gyroSensitivity + gyroNoise[1];
        gyroArr[2] = (float)readWord(OUTX_L_G + 4) * gyroSensitivity + gyroNoise[2];

        // magnetometer update
        if (magDetect)
        {
            sensors_event_t event;
            mag.getEvent(&event);

            magArr[0] = event.magnetic.x - centerOffsetX;
            magArr[1] = event.magnetic.y - centerOffsetY;
        }
    }

    // VL530LX readings
    float *getToFReadings()
    {
        return tofAverage;
    }

    // getting LSM6DS3 readings
    float *getGyroReadings()
    {
        return gyroArr;
    }

    // didn't included this to the sensor update as this won't be much frequently needed
    float *getAccelReadings()
    {
        accelArr[0] = (float)readWord(OUTX_L_XL) * accelScaleFact; // Convert to ms^-1
        accelArr[1] = (float)readWord(OUTX_L_XL) * accelScaleFact;
        accelArr[2] = (float)readWord(OUTX_L_XL) * accelScaleFact;

        return accelArr;
    }

    // GY-271 readings
    float getMagReadings()
    {   
        float direction = atan2(magArr[1]*scaleY, magArr[0]*scaleX) * radToDeg;
        if (direction<0){
            direction += 360;
        }
        return direction;
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
    // Magnetometer
    Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
    float magArr[2] = {0.0, 0.0};
    bool magDetect = false;
    float heading = 0;

    // gyro
    float gyroNoise[3] = {-1.8, 0.33, 1.56};
    float gyroArr[3] = {0.0, 0.0, 0.0};

    // accel
    float accelArr[3] = {0.0, 0.0, 0.0};

    // TOF
    VL53L0X tofRight, tofCenter, tofLeft;
    const int tofOffset[3] = {-40, -31, -10};
    int tofReadings[3] = {0, 0, 0};
    int tofArr[3][10] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    int i = 0;
    int sum[3] = {0, 0, 0};
    float tofAverage[3] = {0, 0, 0};
};