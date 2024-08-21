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

  Serial.print("Before change:   KP  ");
  Serial.print(sensors.tempKp, 4);
  Serial.print("   KD  ");
  Serial.print(sensors.tempKd, 4);
  // // Serial.print("  ROT KP  ");
  // // Serial.print(motors.rotKp, 4);
  // // Serial.print("  ROT KD  ");
  // // Serial.println(motors.rotKd, 4);

  delay(6000);

  Serial.print("After change:   KP  ");
  Serial.print(sensors.tempKp, 4);
  Serial.print("   KD  ");
  Serial.print(sensors.tempKd, 4);

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
//   motion.reset_drive_system();
//   motion.start_move(-70, 200, 0, 1000);
//   while (!motion.move_finished())
//   {
//     encoders.update();
//     motion.update();
//     sensors.update();
//     motors.update(motion.velocity(), motion.omega(), sensors.get_steering_feedback());

//     reporter.send();
//   }

//   motion.reset_drive_system();

//   motion.start_move(720, 200, 0, 1000);
//   while (!motion.move_finished())
//   {
//     encoders.update();
//     motion.update();
//     sensors.update();
//     motors.update(motion.velocity(), motion.omega(), sensors.get_steering_feedback());

//     reporter.send();
//   }

//   motion.reset_drive_system();
//   motion.start_turn(180, 400, 0, 1000);

//   while (!motion.turn_finished())
//   {
//     encoders.update();
//     motion.update();
//     sensors.update();
//     motors.update(motion.velocity(), motion.omega(), sensors.get_steering_feedback());

//     reporter.send();
//   }

// int t = millis();
// while (millis()-t < 2000){
//   encoders.update();
//   motion.update();
//   sensors.update();
//   motors.update(0,0,0);
// }
  sensors.update();
  reporter.send();
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

  // Serial.print("Left:");
  // Serial.print(sensors.left_tof);

  // Serial.print(",");
  // Serial.print("LeftCenter:");
  // Serial.print(sensors.center_left_tof);
  // Serial.print(",");
  // Serial.print("RightCenter:");
  // Serial.print(sensors.center_right_tof);

  // Serial.print(",");
  // Serial.print("Right:");
  // Serial.print(sensors.right_tof);
  // Serial.print(",");
  // Serial.print("Steering:");
  // Serial.print(sensors.get_cross_track_error());
  // Serial.print(",");
  // Serial.print("Steering:");
  // Serial.println(sensors.get_steering_feedback());
}
