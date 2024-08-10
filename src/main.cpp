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
  encoders.update();
  sensors.update();

  // for (int i = 0; i < 30; i++)
  // {
  //   motors.set_left_motor_percentage(i);
  //   motors.set_right_motor_percentage(i);
  //   Serial.println(i);
  //   delay(100);
  // }
  // for (int i = 0; i < 60; i++)
  // {
  //   motors.set_left_motor_percentage(30 - i);
  //   motors.set_right_motor_percentage(30 - i);
  //   Serial.println(30 - i);
  //   delay(100);
  // }
  // for (int i = 0; i < 30; i++)
  // {
  //   motors.set_left_motor_percentage(i - 30);
  //   motors.set_right_motor_percentage(i - 30);
  //   Serial.println(i - 30);
  //   delay(100);
  // }

  // motors.set_left_motor_percentage(100);
  // delay(100);
  // motors.set_left_motor_percentage(-100);


  // Serial.print("Distance   :  ");
  // Serial.print(encoders.robotDistance());
  // Serial.print("    Angle   :  ");
  // Serial.print(encoders.robotAngle());
  // Serial.print("    Left  RPS :  ");
  // Serial.print(encoders.leftRPS());
  // Serial.print("    Right   :  ");
  // Serial.println(encoders.rightRPS());

 
  communication.send("Distance:"+String(encoders.robotDistance()));
  communication.send("Angle:"+String(encoders.robotAngle()));
  communication.send("Left RPS:"+String(encoders.leftRPS()));
  communication.send("Right RPS:"+String(encoders.rightRPS()));

  float *gyroData = sensors.getGyroReadings();

  float gyroX = gyroData[0];
  float gyroY = gyroData[1];
  float gyroZ = gyroData[2];

  communication.send("Gyro X:"+String(gyroX));
  communication.send("Gyro Y:"+String(gyroY));
  communication.send("Gyro Z:"+String(gyroZ));


  float* tofData = sensors.getToFReadings();
  float tofRight = tofData[0];
  float tofCenter = tofData[1];
  float tofLeft = tofData[2];

  communication.send("ToF Right:"+String(tofRight));
  communication.send("ToF Center:"+String(tofCenter));
  communication.send("ToF Left:"+String(tofLeft));

  float direction = sensors.getMagReadings();

  communication.send("Direction:"+String(direction));
  communication.send(String('/'));
  delay(100);

}
