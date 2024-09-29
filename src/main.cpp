#include <Arduino.h>
#include "Ticker.h"
#include "encoders.h"
#include "motors.h"
#include "sensors.h"
#include "reporting.h"
#include "config.h"
#include "motion.h"
#include "profile.h"
#include "mouse.h"
#include "analog.h"
#include "nvs.h"
#include "indicators.h"

Encoders encoders;
Motors motors;
Sensors sensors;
Reporting reporter;
Profile forward;
Profile rotation;
Motion motion;
Mouse mouse;
Maze maze;
Analog analog;
NVS nvs;
Indicators indicators;

Reporting *Reporting::instance = nullptr; // Initialize the static member

Ticker sendTicker;

void setup()
{
  Serial.begin(115200);

  encoders.begin();
  encoders.reset();
  motors.begin();
  sensors.begin();
  indicators.begin();
  maze.initialise();
  mouse.init();
  indicators.begin();

  // Set up a timer to call reporter.send() every 500ms (or any other interval)
  sendTicker.attach(0.0185, []()
                    {
                      encoders.update();
                      sensors.update();
                      motion.update();
                      analog.batteryRead();
                      motors.update(motion.velocity(), motion.omega(), sensors.get_steering_feedback());
                    });

  indicators.customBlink_iter(400, 400, 3);
  indicators.batteryLowIndicator();

  motion.reset_drive_system();
}

void loop()
{ 
  if (analog.switchRead() == 1) //fast search
  {
    mouse.run_acc = SEARCH_ACCELERATION;
    mouse.run_speed = SEARCH_SPEED;

    mouse.search_maze();
    nvs.saveArrays();
    indicators.customBlink_iter(300, 100, 5);
    mouse.search_come_back();
    indicators.customBlink_iter(300, 100, 10);
    nvs.saveArrays();
  }


  else if (analog.switchRead() == 2)   //fast final run
  {
    sensors.steering_kp = STEERING_KP_FINAL;
    sensors.steering_kd = STEERING_KD_FINAL;

    motors.maxMotorPercentage = MAX_MOTOR_PERCENTAGE_FINAL;
    motors.fwdKp = FWD_KP_FINAL;
    motors.fwdKd = FWD_KD_FINAL;
    motors.rotKp = ROT_KP_FINAL;
    motors.rotKd = FWD_KD_FINAL;

    mouse.run_acc = FINAL_ACCERLERATION;
    mouse.run_speed = FINAL_SPEED;

    mouse.offset_180 = OFFSET_180_FINAL;
    mouse.offset_90 = OFFSET_90_FINAL;

    nvs.loadArrays();
    mouse.run_maze();
    indicators.backToBack();
    mouse.run_come_back();
  }


  else if (analog.switchRead() == 3)    //Slow final run;
  { 
    sensors.steering_kp = 0.9;
    sensors.steering_kd = 14;

    motors.maxMotorPercentage = MAX_MOTOR_PERCENTAGE;
    motors.fwdKp = FWD_KP_SMALL;
    motors.fwdKd = FWD_KD_SMALL;
    motors.rotKp = ROT_KP_90;
    motors.rotKd = ROT_KD_90;

    mouse.offset_180 = -6;
    mouse.offset_90 = -6;

    mouse.run_speed = SEARCH_SPEED_SLOW;
    mouse.run_acc = SEARCH_ACCELERATION;


    nvs.loadArrays();
    mouse.run_maze();
    indicators.backToBack();
    mouse.run_come_back();
  }
  else if (analog.switchRead() == 4) //Slow search
  {
    sensors.steering_kp = 0.9;
    sensors.steering_kd = 14;

    motors.maxMotorPercentage = MAX_MOTOR_PERCENTAGE;
    motors.fwdKp = FWD_KP_SMALL;
    motors.fwdKd = FWD_KD_SMALL;
    motors.rotKp = ROT_KP_90;
    motors.rotKd = ROT_KD_90;

    mouse.offset_180 = -6;
    mouse.offset_90 = -7;


    mouse.run_speed = SEARCH_SPEED_SLOW;
    mouse.run_acc = SEARCH_ACCELERATION;

    //adjust the parameters to match the slow parameters
    mouse.search_maze();
    nvs.saveArrays();
    indicators.customBlink_iter(300, 100, 5);
    mouse.search_come_back();
    indicators.customBlink_iter(300, 100, 10);
    nvs.saveArrays();
  }
  else
  {
    // Serial.println("None");
  }

  motors.stop();
  while (true)
  {
    indicators.customBlink_iter(400, 400, 5);
  }
}
