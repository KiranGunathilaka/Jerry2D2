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

  for (int i = 0; i <= 20; i++)
  {
    float l = 0;
    float r = 0;
    for (int j = 0; j < 16; j++)
    {
      encoders.update();
      motors.update(250, 0);

      reporter.send(encoders.robotDistance());
      reporter.send(encoders.robotAngle());

      FWD_KP = reporter.command.floatCmd;
    }
  }
}
