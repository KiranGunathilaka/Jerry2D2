#pragma once

#include <Arduino.h>
#include <config.h>
#include <Wire.h>

#include <VL53L0X.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

enum
{
    STEER_NORMAL,
    STEER_LEFT_WALL,
    STEER_RIGHT_WALL,
    STEERING_OFF,
};

class Sensors;

extern Sensors sensors;

class Sensors
{
public:
    //remove after adjusting
    float tempKp = STEERING_KP;
    float tempKd = STEERING_KD;

    volatile bool frontWallExist;
    volatile bool leftWallExist;
    volatile bool rightWallExist;

    float rightDistance, leftDistance;

    uint8_t steering_mode = STEER_NORMAL;

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
    const int tofOffset[4] = {-10, -45, -42, -18};
    volatile float left_tof, right_tof, center_right_tof, center_left_tof;

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

    int get_front_sum()
    {
        return int(frontSum);
    };

    int get_front_diff()
    {
        return int(frontDiff);
    };

    float get_steering_feedback()
    {
        return steering_adjustment;
    };

    float get_cross_track_error()
    {
        return cross_track_error;
    };

    void update()
    {
        // tof update
        // int t = millis();

        prevLeft = left_tof;
        left_tof = (float)(prevLeft + tofLeft.readRangeContinuousMillimeters() + tofOffset[0]) / 2;

        prevCentLeft = center_left_tof;
        center_left_tof = (float)(prevCentLeft + tofCenLeft.readRangeContinuousMillimeters() + tofOffset[1]) / 2;

        prevCentRight = center_right_tof;
        center_right_tof = (float)(prevCentRight + tofCenRight.readRangeContinuousMillimeters() + tofOffset[2]) / 2;

        prevRight = right_tof;
        right_tof = (float)(prevRight + tofRight.readRangeContinuousMillimeters() + tofOffset[3]) / 2;

        left_tof = abs(prevLeft - left_tof) > 3 ? left_tof : prevLeft;
        right_tof = abs(prevRight - right_tof) > 3 ? right_tof : prevRight;



        // magnetometer update
        if (magDetect)
        {
            sensors_event_t event;
            mag.getEvent(&event);

            magArr[0] = event.magnetic.x - centerOffsetX;
            magArr[1] = event.magnetic.y - centerOffsetY;
        }

        // t -= millis();
        // Serial.println(t);

        // wall detection
        rightWallExist = right_tof < RIGHT_DISTANCE_THRESHOLD;
        leftWallExist = left_tof < LEFT_DISTANCE_THRESHOLD;

        frontSum = center_left_tof + center_right_tof;
        frontDiff = center_left_tof - center_right_tof;
        frontWallExist = frontSum < FRONT_THRESHOLD;


        float rfd = center_right_tof + 25.0;
        float rsd = right_tof+ 45.0;
        float rCosX = sin(38.6)*rfd/sqrt(rfd*rfd  + rsd*rsd - 2*rfd*rsd*cos(38.6 * DEG_TO_RAD));
        rightDistance = rCosX*rsd -20.0 -45.0 * rCosX;

        float lfd = center_left_tof + 30.0;
        float lsd = left_tof+ 45.0;
        Serial.print(lfd);
        Serial.print(" ");
        Serial.println(lsd);
        float lCosX = sin(47.0)*lfd/sqrt(lfd*lfd  + lsd*lsd - 2*lfd*lsd*cos(47.0 * DEG_TO_RAD));
        leftDistance = lCosX * lsd - 45.0 * lCosX;

        // steering
        float error = 0;
        float rightError = SIDE_DISTANCE - right_tof;
        float leftError = SIDE_DISTANCE - left_tof;

        if (steering_mode == STEER_NORMAL)
        {
            if (sensors.leftWallExist && sensors.rightWallExist)
            {
                error = leftError - rightError;
            }
            else if (sensors.leftWallExist)
            {
                error = 2 * leftError;
            }
            else if (sensors.rightWallExist)
            {
                error = -2 * rightError;
            }
        }
        else if (steering_mode == STEER_LEFT_WALL)
        {
            error = 2 * leftError;
        }
        else if (steering_mode == STEER_RIGHT_WALL)
        {
            error = -2 * rightError;
        }

        cross_track_error = error;
        calculate_steering_adjustment();
    }

    float calculate_steering_adjustment()
    {
        // always calculate the adjustment for testing. It may not get used.
        float pTerm = tempKp * cross_track_error;
        float dTerm = tempKd * (cross_track_error - last_steering_error);
        float adjustment = (pTerm + dTerm) * encoders.loopTime_s();
        adjustment = constrain(adjustment, -STEERING_ADJUST_LIMIT, STEERING_ADJUST_LIMIT);
        last_steering_error = cross_track_error;
        steering_adjustment = adjustment;
        return adjustment;
    }

    void set_steering_mode(uint8_t mode)
    {
        last_steering_error = cross_track_error;
        steering_adjustment = 0;
        steering_mode = mode;
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
    // variables for steering
    float last_steering_error = 0;
    volatile float cross_track_error;
    volatile float steering_adjustment;
    volatile float frontSum;
    volatile float frontDiff;

    float prevLeft, prevRight, prevCentLeft, prevCentRight;
};