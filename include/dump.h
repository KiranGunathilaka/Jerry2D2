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

//   #include <stack>

// class Mouse
// {
// public:
//     // ... (keep existing public members and functions)

//     void search()
//     {
//         motion.reset_drive_system();
//         cover_initial_distance();

//         update_map(); // mapping the start cell
//         update_search_wall_state();

//         maze.set_mask(MASK_OPEN);

//         std::stack<Location> path_stack;
//         path_stack.push(m_location);

//         while (!path_stack.empty())
//         {
//             Location current = path_stack.top();

//             if (current == TARGET)
//             {
//                 Serial.println("Target reached!");
//                 break;
//             }

//             bool moved = false;
//             for (int h = NORTH; h < HEADING_COUNT; h++)
//             {
//                 Heading next_heading = static_cast<Heading>(h);
//                 if (can_move_to(current, next_heading))
//                 {
//                     move_to(current.neighbour(next_heading), next_heading);
//                     path_stack.push(current.neighbour(next_heading));
//                     moved = true;
//                     break;
//                 }
//             }

//             if (!moved)
//             {
//                 // Backtrack
//                 path_stack.pop();
//                 if (!path_stack.empty())
//                 {
//                     Location prev = path_stack.top();
//                     Heading backtrack_heading = get_heading_to(prev);
//                     move_to(prev, backtrack_heading);
//                 }
//             }
//         }

//         optimize_path(path_stack);
//     }

// private:
//     // ... (keep existing private members and functions)

//     bool can_move_to(const Location& from, Heading heading)
//     {
//         Location to = from.neighbour(heading);
//         return to.is_in_maze() && !maze.is_wall(from, heading) && !maze.cell_is_visited(to);
//     }

//     void move_to(const Location& to, Heading new_heading)
//     {
//         // Turn to face the new direction
//         int turn_angle = get_turn_angle(m_heading, new_heading);
//         if (turn_angle != 0)
//         {
//             if (turn_angle > 0)
//                 turn_left();
//             else if (turn_angle < 0)
//                 turn_right();
//             else
//                 turn_180();
//         }

//         // Move forward
//         from_cell_to_cell();

//         // Update mouse state
//         m_location = to;
//         m_heading = new_heading;

//         // Update maze information
//         update_map();
//         update_search_wall_state();
//     }

//     Heading get_heading_to(const Location& to)
//     {
//         if (to.x < m_location.x) return WEST;
//         if (to.x > m_location.x) return EAST;
//         if (to.y < m_location.y) return SOUTH;
//         if (to.y > m_location.y) return NORTH;
//         return m_heading; // Should never happen
//     }

//     int get_turn_angle(Heading from, Heading to)
//     {
//         int diff = to - from;
//         if (diff == 3) return -90;
//         if (diff == -3) return 90;
//         return diff * 90;
//     }

//     void optimize_path(std::stack<Location>& path_stack)
//     {
//         // Implement path optimization here
//         // For example, you could remove unnecessary back-and-forth movements
//     }
// };