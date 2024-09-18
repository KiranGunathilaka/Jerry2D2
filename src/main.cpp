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

  indicators.begin();
  indicators.backToBack();

  encoders.begin();
  encoders.reset();
  motors.begin();
  sensors.begin();
  indicators.begin();
  maze.initialise();
  mouse.init();
  // reporter.begin();

  // Set up a timer to call reporter.send() every 500ms (or any other interval)
  sendTicker.attach(0.0185, []()
                    {
                      encoders.update();
                      sensors.update();
                      motion.update();
                      analog.batteryRead();
                      motors.update(motion.velocity(), motion.omega(), sensors.get_steering_feedback());
                      // reporter.sendWalls();
                    });

  indicators.customBlink_iter(400, 400, 5);
  indicators.batteryLowIndicator();
  motion.reset_drive_system();
}

void loop()
{

  if (analog.switchRead() == 1)
  {
    mouse.search_maze();
    nvs.saveArrays();

    indicators.customBlink_iter(300, 100, 5);

    mouse.search_come_back();

    indicators.customBlink_iter(300, 100, 10);

    nvs.saveArrays();
  }
  else if (analog.switchRead() == 2)
  {
    nvs.loadArrays();

    sensors.steering_kp = STEERING_KP_FINAL;
    sensors.steering_kd = STEERING_KD_FINAL;

    mouse.run_maze();

    indicators.backToBack();

    mouse.run_come_back();
  }
  else if (analog.switchRead() == 3)
  {
    // Serial.println("3");
  }
  else if (analog.switchRead() == 4)
  {
    // Serial.println("4");
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
