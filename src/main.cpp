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

  indicators.customBlink_iter(400, 400, 5);
  indicators.batteryLowIndicator();

  motion.reset_drive_system();
}

void loop()
{ 
  if (analog.switchRead() == 1) //fast search
  {
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
    mouse.run_speed = SEARCH_SPEED;
    nvs.loadArrays();
    mouse.run_maze();
    indicators.backToBack();
    mouse.run_come_back();
  }
  else if (analog.switchRead() == 4) //Slow search
  {

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
