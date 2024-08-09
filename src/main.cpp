#include <Arduino.h>
#include "encoders.h"
#include "motors.h"
#include "sensors.h"


Encoders encoders;
Motors motors;
Sensors sensors;

void setup() {
  encoders.begin();
  motors.begin();
  sensors.begin();

  Serial.begin(115200);

}

void loop() {
  encoders.update();
  motors.set_left_motor_percentage(100);
  encoders.update();
  delay(1000);
  motors.set_left_motor_percentage(-100);

  Serial.print("Distance   :  ");
  Serial.print(encoders.robotDistance());
  Serial.print("    Angle   :  ");
  Serial.print(encoders.robotAngle());
  Serial.print("    Left   :  ");
  Serial.print(encoders.counterLeft());
  Serial.print("    Right   :  ");
  Serial.println(encoders.counterRight());
}
