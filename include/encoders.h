#pragma once

#include <Arduino.h>
#include <stdint.h>
#include "config.h"

class Encoders;

extern Encoders encoders;

class Encoders
{
public:
    void begin()
    {
        pinMode(LeftEncoderPin1, INPUT_PULLUP);
        pinMode(LeftEncoderPin2, INPUT_PULLUP);

        pinMode(RightEncoderPin1, INPUT_PULLUP);
        pinMode(RightEncoderPin2, INPUT_PULLUP);

        attachInterrupt(digitalPinToInterrupt(LeftEncoderPin1), updateLeftEncoderISR, CHANGE);
        attachInterrupt(digitalPinToInterrupt(LeftEncoderPin2), updateLeftEncoderISR, CHANGE);

        attachInterrupt(digitalPinToInterrupt(RightEncoderPin1), updateRightEncoderISR, CHANGE);
        attachInterrupt(digitalPinToInterrupt(RightEncoderPin2), updateRightEncoderISR, CHANGE);
    }

    void reset()
    {
        noInterrupts();

        encoderCounterLeft = 0;
        encoderCounterRight = 0;
        robot_distance = 0;
        robot_angle = 0;
        time = 0;

        interrupts();
    }

    static void updateLeftEncoderISR()
    {
        encoders.updateLeftEncoder();
    }

    // Update right encoder position (this function will be called by the ISR)
    static void updateRightEncoderISR()
    {
        encoders.updateRightEncoder();
    }

    void updateLeftEncoder()
    {
        int MSB = digitalRead(LeftEncoderPin1); // Most Significant Bit (A)
        int LSB = digitalRead(LeftEncoderPin2); // Least Significant Bit (B)

        int encoded = (MSB << 1) | LSB; // Create a 2-bit value from A and B

        int sum = (lastEncodedLeft << 2) | encoded; // Combine current and previous states

        // Update position based on the transition
        if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
        {
            encoderCounterLeft++;
        }
        else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
        {
            encoderCounterLeft--;
        }

        lastEncodedLeft = encoded; // Save the current state
    }

    void updateRightEncoder()
    {
        int MSB = digitalRead(RightEncoderPin1); // Most Significant Bit (A)
        int LSB = digitalRead(RightEncoderPin2); // Least Significant Bit (B)

        int encoded = (MSB << 1) | LSB; // Create a 2-bit value from A and B

        int sum = (lastEncodedRight << 2) | encoded; // Combine current and previous states

        // Update position based on the transition
        if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
        {
            encoderCounterRight++;
        }
        else if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
        {
            encoderCounterRight--;
        }

        lastEncodedRight = encoded; // Save the current state
    }

    void update()
    {

        int left_delta = 0;
        int right_delta = 0;
        // Make sure values don't change while being read. Be quick.
        noInterrupts();
        left_delta = encoderCounterLeft;
        right_delta = encoderCounterRight;
        encoderCounterLeft = 0;
        encoderCounterRight = 0;
        interrupts();

        float left_change = left_delta * MM_PER_COUNT_LEFT;
        float right_change = right_delta * MM_PER_COUNT_RIGHT;
        fwd_change = 0.5 * (right_change + left_change);
        robot_distance += fwd_change;
        rot_change = (right_change - left_change) * DEG_PER_MM_DIFFERENCE;
        robot_angle += rot_change;
    }

    float robotDistance()
    {
        float distance;

        noInterrupts();
        distance = robot_distance;
        interrupts();

        return distance;
    }

    float robotAngle()
    {
        float angle;

        noInterrupts();
        angle = robot_angle;
        interrupts();

        return angle;
    }

    float robot_fwd_change()
    {
        float distance;
        interrupts();
        distance = fwd_change;
        noInterrupts();
        return distance;
    }

    float robot_rot_change()
    {
        float distance;
        noInterrupts();
        distance = rot_change;
        interrupts();
        return distance;
    }

private:
    volatile int encoderCounterLeft; // Encoder position
    volatile int lastEncodedLeft;    // Last encoded value

    volatile int encoderCounterRight;
    volatile int lastEncodedRight;

    volatile float robot_distance;
    volatile float robot_angle;
    volatile float time;
    // the change in distance or angle in the last tick.
    float fwd_change;
    float rot_change;
};