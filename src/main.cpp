#include <Arduino.h>
#include "encoders.h"

const int aIn1 = 27;
const int aIn2 = 26;

const int bIn1 = 12;
const int bIn2 = 14;

const int pwmA = 25;
const int pwmB = 13;


Encoders encoders;

void setup() {
  encoders.begin();
  encoders.reset();

  pinMode(aIn1, OUTPUT);
  pinMode(aIn2, OUTPUT);


  ledcSetup(0, 30000, 8);  // channel 0, 30000 Hz, 8-bit resolution
  ledcAttachPin(pwmA, 0);  // attach aIn1 to channel 0
  ledcWrite(0, 255); 

  pinMode(bIn1, OUTPUT);
  pinMode(bIn2, OUTPUT);

  ledcSetup(1, 30000, 8);  // channel 1, 30000 Hz, 8-bit resolution
  ledcAttachPin(pwmB, 1);  // attach pwmB to channel 1
  ledcWrite(1, 255); 

  Serial.begin(115200);

}

void loop() {
  encoders.update();

  Serial.println("Moving Forward");
  digitalWrite(aIn1, LOW);
  digitalWrite(aIn2, HIGH);

  Serial.println("Moving Forward");
  digitalWrite(bIn1, LOW);
  digitalWrite(bIn2, HIGH);
  delay(1000);

  encoders.update();

  digitalWrite(aIn1, HIGH);
  digitalWrite(aIn2, LOW);

  digitalWrite(bIn1, HIGH);
  digitalWrite(bIn2, LOW);
  delay(1000);

  Serial.print("Distance   :  ");
  Serial.print(encoders.robotDistance());
  Serial.print("    Angle   :  ");
  Serial.print(encoders.robotAngle());
  Serial.print("    Left   :  ");
  Serial.print(encoders.counterLeft());
  Serial.print("    Right   :  ");
  Serial.println(encoders.counterRight());
}
