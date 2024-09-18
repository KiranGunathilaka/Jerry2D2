#pragma once

#include <Arduino.h>
#include <config.h>
#include <Wire.h>

#include <VL53L0X.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

class Sensors;

extern Sensors sensors;

enum
{
    STEER_NORMAL,
    STEERING_OFF,
};

class Sensors
{
public:
    uint8_t g_steering_mode = STEER_NORMAL;

    float steering_kp = STEERING_KP;
    float steering_kd = STEERING_KD;

    volatile bool frontWallExist;
    volatile bool leftWallExist;
    volatile bool rightWallExist;

    // Magnetometer
    Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
    float magArr[2] = {0.0, 0.0};
    bool magDetect = false;
    float heading = 0;
    int heading_north;

    float direction;
    // gyro
    float gyroNoise[3] = {-1.8, 0.33, 1.56};
    float gyroArr[3] = {0.0, 0.0, 0.0};

    // accel
    float accelArr[3] = {0.0, 0.0, 0.0};

    // TOF
    VL53L0X tofRight, tofCenter, tofLeft;
    const int tofOffset[3] = {-10, -16, -20};
    volatile float left_tof, right_tof, center_tof;

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

        // according to the library this doesn't just reduce range, also reduces accuracy.
        // change to higher value if reading time doesn't affect the control loop
        tofLeft.setMeasurementTimingBudget(20000);
        tofCenter.setMeasurementTimingBudget(20000);
        tofRight.setMeasurementTimingBudget(20000);

        // Magnetometer initialization
        // magDetect = mag.begin();

        // sensors.update();
        // heading_north = getMagReadings();
        // Serial.println(heading_north);
    }

    float get_steering_feedback()
    {
        return m_steering_adjustment;
    };

    float get_cross_track_error()
    {
        return m_cross_track_error;
    };

    // int get_angle_adjustment(int turned)
    // {
    //     return heading_north - getMagReadings() - turned;
    // }

    void update()
    {
        // tof update
        prevLeft = left_tof;
        left_tof = (float)(prevLeft + tofLeft.readRangeContinuousMillimeters() + tofOffset[0]) / 2.0;

        prevCenter = center_tof;
        center_tof = (float)(prevCenter + tofCenter.readRangeContinuousMillimeters() + tofOffset[1]) / 2.0;

        prevRight = right_tof;
        right_tof = (float)(prevRight + tofRight.readRangeContinuousMillimeters() + tofOffset[2]) / 2.0;

        left_tof = abs(prevLeft - left_tof) > 3 ? left_tof : prevLeft;
        right_tof = abs(prevRight - right_tof) > 3 ? right_tof : prevRight;
        center_tof = abs(prevCenter - center_tof) > 3 ? center_tof : prevCenter;

        // wall detection
        rightWallExist = right_tof < RIGHT_DISTANCE_THRESHOLD;
        leftWallExist = left_tof < LEFT_DISTANCE_THRESHOLD;
        frontWallExist = center_tof < FRONT_THRESHOLD;

        int error = 0;
        int right_error = SIDE_DISTANCE - right_tof;
        int left_error = SIDE_DISTANCE - left_tof;
        if (g_steering_mode == STEER_NORMAL)
        {
            if (sensors.leftWallExist && sensors.rightWallExist)
            {
                error = left_error - right_error;
            }
            else if (sensors.leftWallExist)
            {
                error = 2 * left_error;
            }
            else if (sensors.rightWallExist)
            {
                error = -2 * right_error;
            }
        }
        m_cross_track_error = error;
        calculate_steering_adjustment();

        // magnetometer update
        // if (magDetect)
        // {
        //     sensors_event_t event;
        //     mag.getEvent(&event);

        //     magArr[0] = event.magnetic.x - centerOffsetX;
        //     magArr[1] = event.magnetic.y - centerOffsetY;
        // }
        // direction = atan2(magArr[1] * scaleY, magArr[0] * scaleX) * radToDeg;
        // if (direction < 0)
        // {
        //     direction += 360;
        // }
    }

    float calculate_steering_adjustment()
    {
        // always calculate the adjustment for testing. It may not get used.
        float pTerm = steering_kp * m_cross_track_error;
        float dTerm = steering_kd * (m_cross_track_error - last_steering_error);
        float adjustment = (pTerm + dTerm) * encoders.loopTime_s();
        adjustment = constrain(adjustment, -STEERING_ADJUST_LIMIT, STEERING_ADJUST_LIMIT);
        last_steering_error = m_cross_track_error;
        m_steering_adjustment = adjustment;
        return adjustment;
    }

    void set_steering_mode(uint8_t mode)
    {
        last_steering_error = m_cross_track_error;
        m_steering_adjustment = 0;
        g_steering_mode = mode;
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
    // int getMagReadings()
    // {
    //     return (int)direction;
    // }

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
    volatile float m_cross_track_error;
    volatile float m_steering_adjustment;
    volatile float frontSum;
    volatile float frontDiff;

    float prevLeft, prevRight, prevCenter;
};