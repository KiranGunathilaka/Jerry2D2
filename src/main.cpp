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

int omega, speed;

void setup()
{
  Serial.begin(115200);

  encoders.begin();
  encoders.reset();
  motors.begin();
  sensors.begin();
  reporter.begin();

  motion.reset_drive_system();

  // Serial.print("Before change:  FWD KP  ");
  // Serial.print(motors.fwdKp, 4);
  // Serial.print("  FWD KD  ");
  // Serial.print(motors.fwdKd, 4);
  // Serial.print("  ROT KP  ");
  // Serial.print(motors.rotKp, 4);
  // Serial.print("  ROT KD  ");
  // Serial.println(motors.rotKd, 4);

  // delay(6000);

  // Serial.print("After change:  FWD KP  ");
  // Serial.print(motors.fwdKp, 4);
  // Serial.print("  FWD KD  ");
  // Serial.print(motors.fwdKd, 4);
  // Serial.print("  ROT KP  ");
  // Serial.print(motors.rotKp, 4);
  // Serial.print("  ROT KD  ");
  // Serial.println(motors.rotKd, 4);

  // speed = reporter.speed;
  // omega = reporter.omega;

  // Serial.print("Speed   ");
  // Serial.print(speed);
  // Serial.print("   omega  ");
  // Serial.println(omega);
}

void loop()
{
  // motion.start_move(300, 400, 0, 500);
  // while (!motion.move_finished())
  // {
  //   encoders.update();
  //   motion.update();
  //   sensors.update();
  //   motors.update(motion.velocity(), motion.omega());

  //   reporter.send(encoders.robot_speed(), encoders.robot_omega());
  // }

  // motion.reset_drive_system();
  // motion.start_turn(90, 360, 0, 120);

  // while (!motion.turn_finished())
  // {
  //   encoders.update();
  //   motion.update();
  //   sensors.update();
  //   motors.update(motion.velocity(), motion.omega());

  //   reporter.send(encoders.robot_speed(), encoders.robot_omega());
  // }
  // while (millis() % 20000 < 10000)
  // {
  //   encoders.update();
  //   motors.update(speed, omega);
  //   delay(2);
  //   reporter.send(encoders.robot_speed(), encoders.robot_omega());
  // }

  // while (millis() % 20000 >= 10000)
  // {
  //   encoders.update();
  //   motors.update(-speed, omega);
  //   delay(2);
  //   reporter.send(encoders.robot_speed(), encoders.robot_omega());
  // }
  sensors.update();
  int *tofReadings = sensors.getToFReadings();

  Serial.print("Left : ");
  Serial.print(tofReadings[0]);
  Serial.print("  Left Center : ");
  Serial.print(tofReadings[1]);
  Serial.print("  Right Center: ");
  Serial.print(tofReadings[2]);
  Serial.print("  Right : ");
  Serial.println(tofReadings[3]);
}
