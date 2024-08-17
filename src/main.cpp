#include <Arduino.h>
#include "encoders.h"
#include "motors.h"
#include "sensors.h"
#include "reporting.h"
#include "config.h"

Encoders encoders;
Motors motors;
Sensors sensors;
Reporting reporter;

Reporting *Reporting::instance = nullptr; // Initialize the static member

void setup()
{
  Serial.begin(115200);
    
  encoders.begin();
  encoders.reset();
  motors.begin();
  sensors.begin();
  reporter.begin();

}

void loop()
{
  // encoders.update();
  sensors.update();

}
