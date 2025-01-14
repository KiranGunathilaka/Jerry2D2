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
#include "calibarate.h"

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
Calibaration calibrate;

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
  // reporter.begin();

  // Set up a timer to call reporter.send() every 500ms (or any other interval)
  sendTicker.attach(0.0185, []()
                    {
                      encoders.update();
                      //sensors.update();
                      //motion.update();
                      analog.batteryRead();
                      //motors.update(motion.velocity(), motion.omega(), sensors.get_steering_feedback());
                       });

  indicators.customBlink_iter(400, 400, 2);
  indicators.batteryLowIndicator();

  //Serial.print(analog.batteryRead());

  motion.reset_drive_system();

  calibrate.runMotorCalibration();
}

void setParameters(int runAcc, int runSpeed, float strKp, float strKd,
                   float off90, float off180, float fwdKp, float fwdKd, float rotKp, float rotKd,
                   float mmp = MAX_MOTOR_PERCENTAGE_FINAL)
{
  mouse.run_acc = runAcc;
  mouse.run_speed = runSpeed;

  sensors.steering_kp = strKp;
  sensors.steering_kd = strKd;

  motors.fwdKp = fwdKp;
  motors.fwdKd = fwdKd;
  motors.rotKp = rotKp;
  motors.rotKd = rotKd;

  mouse.offset_90 = off90;
  mouse.offset_180 = off180;

  motors.maxMotorPercentage = mmp;
}

void loop()
{

  if (analog.switchRead() == 1) // fast search
  {
    setParameters(SEARCH_ACCELERATION_FAST, SEARCH_SPEED_FAST, STEERING_KP_SEARCH_FAST, STEERING_KD_SEARCH_FAST,
                  OFFSET_90_SEARCH, OFFSET_180_SEARCH, FWD_KP_FINAL, FWD_KD_FINAL, ROT_KP_FINAL,
                  ROT_KD_FINAL, MAX_MOTOR_PERCENTAGE_FINAL);

    mouse.search_maze();
    mouse.search_come_back();
  }

  else if (analog.switchRead() == 2) // fast final run
  {
    mouse.stop_distance = 75.0;
    mouse.absolute_stop = 60.0;

    setParameters(FINAL_ACCERLERATION_FAST, FINAL_SPEED_FAST, STEERING_KP_FINAL_FAST, STEERING_KD_FINAL_FAST,
                  OFFSET_90_FINAL, OFFSET_180_FINAL, FWD_KP_FINAL, FWD_KD_FINAL, ROT_KP_FINAL,
                  ROT_KD_FINAL, MAX_MOTOR_PERCENTAGE_FINAL);

    nvs.loadArrays();
    mouse.run_maze();

    mouse.run_come_back();
  }

  else if (analog.switchRead() == 3) // Slow final run;
  {
    setParameters(FINAL_ACCERLERATION_SLOW, FINAL_SPEED_SLOW, STEERING_KP_FINAL_FAST, STEERING_KD_FINAL_FAST,
                  OFFSET_90_FINAL, OFFSET_180_FINAL, FWD_KP_SMALL, FWD_KD_SMALL, ROT_KP_90,
                  ROT_KD_90, MAX_MOTOR_PERCENTAGE_SEARCH);

    nvs.loadArrays();
    mouse.run_maze();
    indicators.backToBack();
    mouse.run_come_back();
  }

  else if (analog.switchRead() == 4) // Slow search
  {

    setParameters(SEARCH_ACCELERATION_SLOW, SEARCH_SPEED_SLOW, STEERING_KP_SEARCH_SLOW, STEERING_KD_SEARCH_SLOW,
                  OFFSET_90_SEARCH, OFFSET_180_SEARCH, FWD_KP_SMALL, FWD_KD_SMALL, ROT_KP_90,
                  ROT_KD_90, MAX_MOTOR_PERCENTAGE_SEARCH);

    // adjust the parameters to match the slow parameters
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
