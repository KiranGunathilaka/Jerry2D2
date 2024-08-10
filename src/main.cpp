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
  //encoders.update();
  sensors.update();


  for (int i=0; i<=20;i++){
      float l = 0;
      float r = 0;
      for(int j=0; j<40;j++){
         motors.set_left_motor_percentage(motors.left_feed_forward_percentage(110.0));
         motors.set_right_motor_percentage(motors.left_feed_forward_percentage(110.0));
         delay(100);
         encoders.update();
         l += encoders.leftRPS();
         r += encoders.rightRPS();
      }
      int lavg = l/40;
      int ravg = r/40;
      Serial.print(i);
      Serial.print(" l ");
      Serial.print(motors.left_feed_forward_percentage(110.0));
      Serial.print("  r  ");
      Serial.println(ravg);
      // Serial.print(" speed ");
      // Serial.println(encoders.robot_speed());


      
    }
}
