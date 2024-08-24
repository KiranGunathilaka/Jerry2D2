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
  //maze.set_goal(TARGET);

  // Set up a timer to call reporter.send() every 500ms (or any other interval)
  //sendTicker.attach(0.015, []()  { reporter.send(); });
  delay(4000);
 
}

void loop()
{
  mouse.search();

  while(true){
    motors.stop();
  }


}
