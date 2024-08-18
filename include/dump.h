 //****************************************************************MOTOR TESTING******************** */
  // for (int i = 0; i < 30; i++)
  // {
  //   motors.set_left_motor_percentage(i);
  //   motors.set_right_motor_percentage(i);
  //   Serial.println(i);
  //   delay(100);
  // }
  // for (int i = 0; i < 60; i++)
  // {
  //   motors.set_left_motor_percentage(30 - i);
  //   motors.set_right_motor_percentage(30 - i);
  //   Serial.println(30 - i);
  //   delay(100);
  // }
  // for (int i = 0; i < 30; i++)
  // {
  //   motors.set_left_motor_percentage(i - 30);
  //   motors.set_right_motor_percentage(i - 30);
  //   Serial.println(i - 30);
  //   delay(100);
  // }

  // motors.set_left_motor_percentage(100);
  // delay(100);
  // motors.set_left_motor_percentage(-100);

//********************************************************************SEND SENSOR DATA************************ */
  // Serial.print("Distance   :  ");
  // Serial.print(encoders.robotDistance());
  // Serial.print("    Angle   :  ");
  // Serial.print(encoders.robotAngle());
  // Serial.print("    Left  RPS :  ");
  // Serial.print(encoders.leftRPS());
  // Serial.print("    Right   :  ");
  // Serial.println(encoders.rightRPS());

 
  // communication.send("Distance:"+String(encoders.robotDistance()));
  // communication.send("Angle:"+String(encoders.robotAngle()));
  // communication.send("Left RPS:"+String(encoders.leftRPS()));
  // communication.send("Right RPS:"+String(encoders.rightRPS()));

  // float *gyroData = sensors.getGyroReadings();

  // float gyroX = gyroData[0];
  // float gyroY = gyroData[1];
  // float gyroZ = gyroData[2];

  // communication.send("Gyro X:"+String(gyroX));
  // communication.send("Gyro Y:"+String(gyroY));
  // communication.send("Gyro Z:"+String(gyroZ));


  // float* tofData = sensors.getToFReadings();
  // float tofRight = tofData[0];
  // float tofCenter = tofData[1];
  // float tofLeft = tofData[2];

  // communication.send("ToF Right:"+String(tofRight));
  // communication.send("ToF Center:"+String(tofCenter));
  // communication.send("ToF Left:"+String(tofLeft));

  // float direction = sensors.getMagReadings();

  // communication.send("Direction:"+String(direction));
  // communication.send(String('/'));


  //************************************************read sensors data tof********************************************
  // void update()
  //   {
  //       // tof update
  //       int t = millis();
  //       for (int j = 0; j < 5; j++)
  //       {
  //           tofReadings[0] += tofRight.readRangeContinuousMillimeters();
  //           tofReadings[1] += tofCenRight.readRangeContinuousMillimeters();
  //           tofReadings[2] += tofCenLeft.readRangeContinuousMillimeters();
  //           tofReadings[3] += tofLeft.readRangeContinuousMillimeters();
  //       }

  //       for (int i = 0; i < 4; i++)
  //       {
  //           tofAverage[i] = (float)tofReadings[i] / 5.0;
  //           tofReadings[i] = 0;
  //       }

  //       // tofAverage[0] = tofRight.readRangeContinuousMillimeters();
  //       // tofAverage[1] = tofCenRight.readRangeContinuousMillimeters();
  //       // tofAverage[2] = tofCenLeft.readRangeContinuousMillimeters();
  //       // tofAverage[3] = tofLeft.readRangeContinuousMillimeters();

  //       t -= millis();
  //       Serial.println(t);
  //   }