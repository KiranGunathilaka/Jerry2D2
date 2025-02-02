#pragma once

#include <Arduino.h>
#include "motors.h"
#include "encoders.h"
#include "config.h"

class Calibaration;
extern Calibaration calibaration;

class Calibaration
{
private:
    const int SAMPLES_PER_PERCENTAGE = 50; // Number of samples to average for each percentage
    const int STABILIZE_DELAY = 500;       // Time in ms to wait for motor speed to stabilize
    const float PERCENTAGE_STEP = 1.0;     // Step size for percentage increments

public:
    struct MotorData
    {
        float percentage;
        float left_rps;
        float right_rps;
    };

    // Motor feed forward calibration functions
    //  Function to collect data for a single percentage value
    MotorData measureAtPercentage(float percentage)
    {
        MotorData data;
        data.percentage = percentage;

        // Set all motors to the same percentage
        motors.set_left_motor_percentage(percentage);
        motors.set_right_motor_percentage(percentage);

        // Wait for speed to stabilize
        delay(STABILIZE_DELAY);

        // Take multiple samples and average them
        float l_rps = 0, r_rps = 0;

        for (int i = 0; i < SAMPLES_PER_PERCENTAGE; i++)
        {
            l_rps += abs((float) encoders.leftRPS());
            r_rps += abs((float) encoders.rightRPS());

            delay(20); // Small delay between samples
        }

        // Calculate averages
        data.left_rps = l_rps / SAMPLES_PER_PERCENTAGE;
        data.right_rps = r_rps / SAMPLES_PER_PERCENTAGE;

        return data;
    }

    // Function to run a complete calibration sequence
    void runMotorCalibration()
    {
        Serial.println("Starting motor calibration...");
        Serial.println("Percentage, Left , Right ");

        // Test from MIN_MOTOR_PERCENTAGE to MAX_MOTOR_PERCENTAGE
        for (float percentage = MIN_MOTOR_PERCENTAGE; percentage <= MAX_MOTOR_PERCENTAGE_FINAL; percentage += PERCENTAGE_STEP)
        {

            MotorData data = measureAtPercentage(percentage);
            printMotorData(data);
            delay(30); // Brief pause between measurements
        }


        delay(500);
        // Test negative percentages
        for (float percentage = -MIN_MOTOR_PERCENTAGE; percentage >= -MAX_MOTOR_PERCENTAGE_FINAL; percentage -= PERCENTAGE_STEP)
        {

            MotorData data = measureAtPercentage(percentage);
            printMotorData(data);
            delay(30);
        }

        delay(500);

        // Stop motors after calibration
        motors.stop();
        Serial.println("Calibration complete!");
    }

    // Don't need to move this function to the printer class, as this will be only used one time for the entire project 
    //unless current limited power sources and motors didn't change
    void printMotorData(const MotorData &data)
    {
        char buffer[100];
        snprintf(buffer, sizeof(buffer),
                 "%6.2f, %6.2f, %6.2f",
                 data.percentage,
                 data.left_rps * MM_PER_ROTATION/ PULSES_PER_ROTATION,
                 data.right_rps * MM_PER_ROTATION/ PULSES_PER_ROTATION);
        Serial.println(buffer);
    }
};