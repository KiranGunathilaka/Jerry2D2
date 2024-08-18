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
        Wire.begin(SDA_pin, SCL_pin, 400000); // SDA , SCL

        pinMode(23, INPUT_PULLUP);
        pinMode(22, INPUT_PULLUP);
        writeByteGyro(CTRL1_XL, 0x50); // set the accel range to +- 4g
        writeByteGyro(CTRL2_G, 0x50);  // Set the Gyro range to +- 500 dps

        byte ctrl1 = readByteGyro(CTRL1_XL);
        byte ctrl2 = readByteGyro(CTRL2_G);

        // ToF address change
        pinMode(ToF_XSHUT_Right, OUTPUT);
        pinMode(ToF_XSHUT_CenRight, OUTPUT);
        pinMode(ToF_XSHUT_CenLeft, OUTPUT);
        pinMode(ToF_XSHUT_Left, OUTPUT);

        digitalWrite(ToF_XSHUT_Right, LOW);
        digitalWrite(ToF_XSHUT_CenRight, LOW);
        digitalWrite(ToF_XSHUT_CenLeft, LOW);
        digitalWrite(ToF_XSHUT_Left, LOW);

        Serial.begin(115200);

        // changing address of 1st tof
        digitalWrite(ToF_XSHUT_Right, HIGH);
        delay(30);
        tofRight.init(true);
        delay(30);
        tofRight.setAddress(TOF_RIGHT_ADD);

        // changing address of 2nd tof
        digitalWrite(ToF_XSHUT_CenRight, HIGH);
        delay(30);
        tofCenRight.init(true);
        delay(30);
        tofCenRight.setAddress(TOF_CENRIGHT_ADD);

        // changing the third ToF
        digitalWrite(ToF_XSHUT_CenLeft, HIGH);
        delay(30);
        tofCenLeft.init(true);
        delay(30);
        tofCenLeft.setAddress(TOF_CENLEFT_ADD);

        // changing address of 4th tof
        digitalWrite(ToF_XSHUT_Left, HIGH);
        delay(30);
        tofLeft.init(true);
        delay(30);
        tofLeft.setAddress(TOF_LEFT_ADD);

        tofRight.startContinuous();
        tofCenRight.startContinuous();
        tofCenLeft.startContinuous();
        tofLeft.startContinuous();

        // according to the library this doesn't just reduce range, also reduces accuracy.
        // change to higher value if reading time doesn't affect the control loop
        tofLeft.setMeasurementTimingBudget(20000);
        tofCenLeft.setMeasurementTimingBudget(20000);
        tofCenRight.setMeasurementTimingBudget(20000);
        tofRight.setMeasurementTimingBudget(20000);

        // Magnetometer initialization
        magDetect = mag.begin();
    }

    void update()
    {
        // tof update
        int t = millis();

        tof[0] = tofLeft.readRangeContinuousMillimeters() + tofOffset[0];
        tof[1] = tofCenLeft.readRangeContinuousMillimeters() + tofOffset[1];
        tof[2] = tofCenRight.readRangeContinuousMillimeters() + tofOffset[2];
        tof[3] = tofRight.readRangeContinuousMillimeters() + tofOffset[3];

        // magnetometer update
        if (magDetect)
        {
            sensors_event_t event;
            mag.getEvent(&event);

            magArr[0] = event.magnetic.x - centerOffsetX;
            magArr[1] = event.magnetic.y - centerOffsetY;
        }

        t -= millis();
        Serial.println(t);
    }

    // VL530LX readings
    int *getToFReadings()
    {
        return tof;
    }

    // getting LSM6DS3 readings
    float *getGyroReadings()
    {
        // gyro update
        gyroArr[0] = (float)readWord(OUTX_L_G) * gyroSensitivity + gyroNoise[0]; // Convert to dps
        gyroArr[1] = (float)readWord(OUTX_L_G + 2) * gyroSensitivity + gyroNoise[1];
        gyroArr[2] = (float)readWord(OUTX_L_G + 4) * gyroSensitivity + gyroNoise[2];
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
        float direction = atan2(magArr[1] * scaleY, magArr[0] * scaleX) * radToDeg;
        if (direction < 0)
        {
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
    VL53L0X tofRight, tofCenRight, tofCenLeft, tofLeft;
    const int tofOffset[4] = {-25, -45, -42, -32};
    int tof[4] = {0, 0, 0, 0};
};