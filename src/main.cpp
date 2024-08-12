#include <Arduino.h>
#include "encoders.h"
#include "motors.h"
#include "sensors.h"
#include "communication.h"

Encoders encoders;
Motors motors;
Sensors sensors;
Communication communication;

void setup()
{
  encoders.begin();
  encoders.reset();
  motors.begin();
  sensors.begin();
  Serial.begin(115200);
  communication.begin();
}

void loop()
{
  // encoders.update();
  sensors.update();

  for (int i = 0; i <= 20; i++)
  {
    float l = 0;
    float r = 0;
    for (int j = 0; j < 16; j++)
    {
      encoders.update();
      motors.update(250, 0);

      // Serial.print("  given velocity  ");
      // Serial.print(250);
      // Serial.print(" actual velocity ");
      // Serial.print(encoders.robot_speed());
      // Serial.print("  motor percentage  ");
      // Serial.println(motors.left_feed_forward_percentage(250));

      // Serial.print("Distance   :");
      // Serial.print(encoders.robotDistance());
      // Serial.print("   Angle   : ");
      // Serial.println(encoders.robotAngle());
    }
  }
}
