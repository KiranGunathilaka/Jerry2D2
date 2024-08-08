#include <Arduino.h>
#include "encoders.h"

Encoders encoders;

void setup() {
  encoders.begin();
  encoders.reset();

  Serial.begin(115200);

}

void loop() {
  encoders.update();

  

  Serial.println(encoders.robotDistance());
  Serial.println(encoders.robotAngle());


  delay(500);
}
