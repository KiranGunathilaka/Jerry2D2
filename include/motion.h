#pragma once

#include <Arduino.h>
#include "motors.h"
#include "profile.h"

extern Motion motion;

class Motion {
 public:

  void reset_drive_system() {
    motors.stop();
    motors.disable_controllers();
    encoders.reset();
    forward.reset();
    rotation.reset();
    motors.reset_controllers();
    motors.enable_controllers();
  }

  void stop() {
    motors.stop();
  }

  void disable_drive() {
    motors.disable_controllers();
  }

  float position() {
    return forward.position();
  }

  float velocity() {
    return forward.speed();
  }

  float acceleration() {
    return forward.acceleration();
  }

  void set_target_velocity(float velocity) {
    forward.set_target_speed(velocity);
  }

  float angle() {
    return rotation.position();
  }

  float omega() {
    return rotation.speed();
  }

  float alpha() {
    return rotation.acceleration();
  }

  void start_move(float distance, float top_speed, float final_speed, float acceleration) {
    forward.start(distance, top_speed, final_speed, acceleration);
  }

  bool move_finished() {
    return forward.is_finished();
  }

  void move(float distance, float top_speed, float final_speed, float acceleration) {
    forward.move(distance, top_speed, final_speed, acceleration);
  }

  void start_turn(float distance, float top_speed, float final_speed, float acceleration) {
    rotation.start(distance, top_speed, final_speed, acceleration);
  }

  bool turn_finished() {
    return rotation.is_finished();
  }

  void turn(float distance, float top_speed, float final_speed, float acceleration) {
    rotation.move(distance, top_speed, final_speed, acceleration);
  }

  void update() {
    forward.update();
    rotation.update();
  }

  void set_position(float pos) {
    forward.set_position(pos);
  }

  void adjust_forward_position(float delta) {
    forward.adjust_position(delta);
  }

  //***************************************************************************//

  void turn(float angle, float omega, float alpha) {
    // get ready to turn
    rotation.reset();
    rotation.move(angle, omega, 0, alpha);
  }

  void spin_turn(float angle, float omega, float alpha) {
    forward.set_target_speed(0);
    while (forward.speed() != 0) {
      delay(2);
    }
    turn(angle, omega, alpha);
  };

};
