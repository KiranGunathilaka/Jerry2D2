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
      for(int j=0; j<16;j++){
         motors.set_left_motor_percentage(motors.left_feed_forward_percentage(50+j*30));
         motors.set_right_motor_percentage(motors.left_feed_forward_percentage(50+j*30));
         delay(1000);
         encoders.update();
         Serial.println(i);
         Serial.print("  given velocity  ");
         Serial.print(50+j*30);
         Serial.print(" actual velocity ");
         Serial.print(encoders.robot_speed());
         Serial.print("  motor percentage  ");
         Serial.println(motors.left_feed_forward_percentage(50+j*30));
  

         Serial.println(encoders.robotDistance());
         Serial.println(encoders.robotAngle());
         
       }


      
    }
}
