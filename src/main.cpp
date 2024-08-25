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

Encoders encoders;
Motors motors;
Sensors sensors;
Reporting reporter;
Profile forward;
Profile rotation;
Motion motion;
Mouse mouse;
Maze maze;

Reporting *Reporting::instance = nullptr; // Initialize the static member

int omega, speed;
Ticker sendTicker;

void setup()
{
  Serial.begin(115200);

  encoders.begin();
  encoders.reset();
  motors.begin();
  sensors.begin();
  reporter.begin();
  maze.initialise();
  mouse.init();

  // Set up a timer to call reporter.send() every 500ms (or any other interval)
  sendTicker.attach(0.0185, []()
                    { 
                      encoders.update();
                      sensors.update();
                      motion.update();
                      motors.update(motion.velocity(), motion.omega(), sensors.get_steering_feedback());
                      reporter.sendWalls(); });

  delay(2000);
}

void loop()
{
  mouse.search();

  while (true)
  {
    motors.stop();
  }
}
