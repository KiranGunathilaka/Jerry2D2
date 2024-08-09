#include <Arduino.h>
#include "encoders.h"
#include "motors.h"

Encoders encoders;
Motors motors;

void setup() {
  encoders.begin();
  encoders.reset();
  motors.begin();

  Serial.begin(115200);

}

void loop() {
  encoders.update();
for (int i=0;i<30;i++){
    motors.set_left_motor_percentage(i);
    motors.set_right_motor_percentage(i);
    Serial.println(i);
    delay(100);
}
for (int i=0;i<60;i++){
    motors.set_left_motor_percentage(30-i);
    motors.set_right_motor_percentage(30-i);
    Serial.println(30-i);
    delay(100);
}
for (int i=0;i<30;i++){
    motors.set_left_motor_percentage(i-30);
    motors.set_right_motor_percentage(i-30);
    Serial.println(i-30);
    delay(100);
}


}
