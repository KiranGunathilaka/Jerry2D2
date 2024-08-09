#include <Arduino.h>
#include "motors.h"


void setup() {
    motors.begin();
    //encoders.begin();
    Serial.begin(115200);
}
void loop() {
    //int leftDelta, rightDelta= 0, 0;
    //encoders.update(&leftDelta, &rightDelta);
    //Serial.print(leftDelta);
    //Serial.println(rightDelta);
    for (int i = 0 ; i<=20 ; i++){
        motors.set_left_motor_percentage(i);
        motors.set_right_motor_percentage(i);
        Serial.println(i);
        delay(50);
    }
    delay(1000); // Run for 2 seconds
    for (int i = 0 ; i<=20 ; i++){
        motors.set_left_motor_percentage(20-i);
        motors.set_right_motor_percentage(20-i);
        Serial.println(i);
        delay(50);
    }
    delay(1000);
    for (int i = 0 ; i<=20 ; i++){
        motors.set_left_motor_percentage(-i);
        motors.set_right_motor_percentage(-i);
        Serial.println(i);
        delay(50);
    }
    delay(1000);
    for (int i = 0 ; i<=20 ; i++){
        motors.set_left_motor_percentage(-20+i);
        motors.set_right_motor_percentage(-20+i);
        Serial.println(i);
        delay(50);
    }
    delay(1000);
}

