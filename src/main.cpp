#include <Arduino.h>
#include "encoders.h"
#include "motors.h"
#include "sensors.h"
#include "reporting.h"
#include "config.h"
#include "motion.h"
#include "profile.h"

Encoders encoders;
Motors motors;
Sensors sensors;
Reporting reporter;
Profile forward;
Profile rotation;
Motion motion;

Reporting *Reporting::instance = nullptr; // Initialize the static member

void setup()
{
  Serial.begin(115200);

  encoders.begin();
  encoders.reset();
  motors.begin();
  sensors.begin();
  reporter.begin();

  motion.reset_drive_system();
  delay(2000);
}

void loop()
{ 
  // motion.reset_drive_system();
  // motion.start_move(1000, 400, 0, 500);
  // while (!motion.move_finished())
  // {
  //   encoders.update();
  //   motion.update();
  //   sensors.update();
  //   motors.update(motion.velocity(), motion.omega());

  //   Serial.print(motion.velocity());
  //   Serial.print("       ");
  //   Serial.println(motion.omega());
  // }
  

  encoders.update();
  motors.update(300, 0);
  
  // Serial.print(encoders.robot_omega());
  // Serial.print("    ");
  // Serial.println(encoders.robot_speed());

  Serial.println(encoders.robot_speed());
  reporter.send(encoders.robot_speed());

}
