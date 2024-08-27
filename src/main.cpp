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

Ticker sendTicker;

const int SEARCH_SELECT_PIN = 2;
const int RUN_SELECT_PIN = 15;

void setup()
{
  Serial.begin(115200);

  // pinMode(SEARCH_SELECT_PIN, INPUT_PULLUP);
  // pinMode(RUN_SELECT_PIN, INPUT_PULLUP);

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

  delay(4000);
}

void loop()
{
  // int switchStateSearch = digitalRead(SEARCH_SELECT_PIN);
  // int switchStateRun = digitalRead(RUN_SELECT_PIN);

  // Check if the search switch is ON (LOW)
  // if (switchStateSearch == LOW) {
  //   Serial.println("Searching");
  //   delay(5000);
  //   mouse.search();

  //   // Wait until the switch is turned off
  //   while (digitalRead(SEARCH_SELECT_PIN) == LOW) {
  //     Serial.print("Search done");
  //     motors.stop();
  //     delay(100);
  //   }
  // }

  // // Check if the run switch is ON (LOW)
  // if (switchStateRun == LOW) {
  //   Serial.println("Running");
  //   delay(5000);
  //   mouse.run();

  //   // Wait until the switch is turned off
  //   while (digitalRead(RUN_SELECT_PIN) == LOW) {
  //     Serial.println("Run done");
  //     motors.stop();
  //     delay(100);
  //   }
  // }

  mouse.search();

  while (true)
  {
    motors.stop();
    delay(100);
  }

}
