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
  motion.start_move(300, 400, 0, 500);
  while (!motion.move_finished())
  {
    encoders.update();
    motion.update();
    sensors.update();
    motors.update(motion.velocity(), motion.omega());

    reporter.send(motion.velocity());
    reporter.send(motion.omega());
  }

  motion.start_turn(90, 40, 0, 10);

  while (!motion.turn_finished())
  {
    encoders.update();
    motion.update();
    sensors.update();
    motors.update(motion.velocity(), motion.omega());

    reporter.send(motion.velocity());
    reporter.send(motion.omega());
  }
  
  // Serial.print(encoders.robot_omega());
  // Serial.print("    ");
  // Serial.println(encoders.robot_speed());

}
