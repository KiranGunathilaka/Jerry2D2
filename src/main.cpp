#include <Arduino.h>
#include "encoders.h"
#include "motors.h"
#include "sensors.h"

Encoders encoders;
Motors motors;
Sensors sensors;

void setup()
{
  encoders.begin();
  motors.begin();
  delay(50);
  sensors.begin();

  Serial.begin(115200);
}

void loop()
{
  encoders.update();
  motors.set_left_motor_percentage(100);
  delay(100);
  motors.set_left_motor_percentage(-100);

  Serial.print("Distance   :  ");
  Serial.print(encoders.robotDistance());
  Serial.print("    Angle   :  ");
  Serial.print(encoders.robotAngle());
  Serial.print("    Left   :  ");
  Serial.print(encoders.counterLeft());
  Serial.print("    Right   :  ");
  Serial.println(encoders.counterRight());

  float* gyroData = sensors.getGyroReadings();

  float gyroX = gyroData[0];
  float gyroY = gyroData[1];
  float gyroZ = gyroData[2];

  Serial.print("Gyro X: ");
  Serial.print(gyroX);
  Serial.print("   Gyro Y:  ");
  Serial.print(gyroY);
  Serial.print("   Gyro Z:  ");
  Serial.println(gyroZ);
}
