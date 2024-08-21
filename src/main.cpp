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

  // Serial.print("Before change:   KP  ");
  // Serial.print(sensors.tempKp, 4);
  // Serial.print("   KD  ");
  // Serial.print(sensors.tempKd, 4);
  // // Serial.print("  ROT KP  ");
  // // Serial.print(motors.rotKp, 4);
  // // Serial.print("  ROT KD  ");
  // // Serial.println(motors.rotKd, 4);

  // delay(6000);

  // Serial.print("After change:   KP  ");
  // Serial.print(sensors.tempKp, 4);
  // Serial.print("   KD  ");
  // Serial.print(sensors.tempKd, 4);

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
  motion.reset_drive_system();
  sensors.set_steering_mode(STEER_NORMAL);
  int i = 0;
  while (i < 5)
  {
    motion.start_move(180, 400, 0, 1000);
    while (!motion.move_finished())
    {
      encoders.update();
      motion.update();
      sensors.update();
      motors.update(motion.velocity(), motion.omega(), sensors.get_steering_feedback());
      reporter.send();
    }
    i++;
    motors.stop();
  }

  motion.reset_drive_system();
  sensors.set_steering_mode(STEERING_OFF);
  motion.start_turn(90, 360, 0, 3600);
  while (!motion.turn_finished())
  {
    encoders.update();
    motion.update();
    sensors.update();
    motors.update(motion.velocity(), motion.omega(), sensors.get_steering_feedback());

    reporter.send();
  }
  motors.stop();

  motion.reset_drive_system();
  sensors.set_steering_mode(STEER_NORMAL);
  motion.start_move(180, 400, 0, 1000);
  while (!motion.move_finished())
  {
    encoders.update();
    motion.update();
    sensors.update();
    motors.update(motion.velocity(), motion.omega(), sensors.get_steering_feedback());
    reporter.send();
  }
  motors.stop();

  motion.reset_drive_system();
  sensors.set_steering_mode(STEERING_OFF);
  motion.start_turn(90, 360, 0, 3600);
  while (!motion.turn_finished())
  {
    encoders.update();
    motion.update();
    sensors.update();
    motors.update(motion.velocity(), motion.omega(), sensors.get_steering_feedback());

    reporter.send();
  }
  motors.stop();
}
