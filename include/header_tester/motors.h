#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include "config.h"
//#include "encoders.h"

class Motors;

extern Motors motors;

class Motors {
 public:
   void begin(){
        pinMode(LEFT_MOTOR_PWM, OUTPUT);
        pinMode(LEFT_MOTOR_IN1, OUTPUT);
        pinMode(LEFT_MOTOR_IN2, OUTPUT);
        pinMode(RIGHT_MOTOR_IN2, OUTPUT);
        pinMode(RIGHT_MOTOR_IN1, OUTPUT);
        pinMode(RIGHT_MOTOR_PWM, OUTPUT);
        digitalWrite(LEFT_MOTOR_PWM, 0);
        digitalWrite(LEFT_MOTOR_IN1, 0);
        digitalWrite(LEFT_MOTOR_IN2, 0);
        digitalWrite(RIGHT_MOTOR_IN2, 0);
        digitalWrite(RIGHT_MOTOR_IN1, 0);
        digitalWrite(RIGHT_MOTOR_PWM, 0);
        setupPWM();
   }
   void stop() {
    set_left_motor_percentage(0);
    set_right_motor_percentage(0);
  }
  void set_rps(float left_rps, float right_rps){

  }
  void set_left_motor_percentage(float percentage){
    percentage = constrain(percentage, -MAX_MOTOR_PERCENTAGE, MAX_MOTOR_PERCENTAGE);
    //m_left_motor_percentage = percentage;
    int left_pwm = calculate_pwm(percentage);
    set_left_motor_pwm(left_pwm);
  }
  void set_left_motor_pwm(int pwm){
    pwm  = MOTOR_LEFT_POLARITY*pwm;
    if (pwm<0){
        digitalWrite(LEFT_MOTOR_IN1, HIGH);
        digitalWrite(LEFT_MOTOR_IN2, LOW);
        ledcWrite(LEFT_MOTOR_PWM , -pwm + M_BALNCE_PWM);
    }else {
        digitalWrite(LEFT_MOTOR_IN1, LOW);
        digitalWrite(LEFT_MOTOR_IN2, HIGH);
        ledcWrite(LEFT_MOTOR_PWM , pwm + M_BALNCE_PWM);
    }
    }
  int calculate_pwm(float desired_percentage){
    int pwm = MAX_MOTOR_PERCENTAGE*PWM_RESOLUTION*desired_percentage/100;
    return pwm;
  }
  void set_right_motor_percentage(float percentage){
    percentage = constrain(percentage, -MAX_MOTOR_PERCENTAGE, MAX_MOTOR_PERCENTAGE);
    //m_right_motor_percentage = percentage;
    int left_pwm = calculate_pwm(percentage);
    set_right_motor_pwm(left_pwm);
  }
  void set_right_motor_pwm(int pwm){
    pwm  = MOTOR_RIGHT_POLARITY*pwm;
    if (pwm<0){
        digitalWrite(RIGHT_MOTOR_IN1, HIGH);
        digitalWrite(RIGHT_MOTOR_IN2, LOW);
        ledcWrite(RIGHT_MOTOR_PWM , -pwm - M_BALNCE_PWM);
    }else {
        digitalWrite(RIGHT_MOTOR_IN1, LOW);
        digitalWrite(RIGHT_MOTOR_IN2, HIGH);
        ledcWrite(RIGHT_MOTOR_PWM , pwm - M_BALNCE_PWM);
    }
    }
    void setupPWM(){
        ledcAttachChannel(LEFT_MOTOR_PWM, 5000, PWM_RESOLUTION_BITS, 1);
        ledcAttachChannel(RIGHT_MOTOR_PWM, 5000, PWM_RESOLUTION_BITS, 2);
    }

private:
    float m_left_motor_percentage;
    float m_right_motor_percentage;
    float m_previous_fwd_error;
    float m_previous_rot_error;
    float m_fwd_error;
    float m_rot_error;
    float m_velocity;
    float m_omega;
};
#endif