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

Reporting *Reporting::instance = nullptr; // Initialize the static member

Ticker sendTicker;


void setup()
{
  Serial.begin(115200);

  encoders.begin();
  encoders.reset();
  motors.begin();
  sensors.begin();
  //reporter.begin();
  maze.initialise();
  mouse.init();

  // Set up a timer to call reporter.send() every 500ms (or any other interval)
  sendTicker.attach(0.0185, []()
                    { 
                      encoders.update();
                      sensors.update();
                      motion.update();
                      analog.batteryRead();
                      motors.update(motion.velocity(), motion.omega(), sensors.get_steering_feedback());
                      //reporter.sendWalls(); 
                      });
  pinMode(INDICATOR_PIN, OUTPUT);
  for(int i=0 ;i<4 ;i++){
      digitalWrite(INDICATOR_PIN, HIGH);
      delay(500);
      digitalWrite(INDICATOR_PIN, LOW);
      delay(500);
  }
}

void loop()
{
  // if (analog.switchRead() == 1)
  // {
  //   Serial.println("1");
  //   mouse.search();
  // }else if (analog.switchRead() ==2){
  //   //Serial.println("2");
  // }else if (analog.switchRead() ==3){
  //   //Serial.println("3");
  // }else if (analog.switchRead() ==4){
  //   //Serial.println("4");
  // }else{
  //   //Serial.println("None");
  // }
  mouse.search_maze();

  delay(5000);
  mouse.run_to(TARGET);

  motors.stop();
  //analog.batteryRead();
  while(true){
      delay(200);
  }
}
