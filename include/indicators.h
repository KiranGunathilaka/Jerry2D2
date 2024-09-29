#ifndef INDICATORS_H
#define INDICATORS_H

#include <Arduino.h>
#include <stdint.h>
#include "config.h"
#include "Buzzer.h"
#include "analog.h"
#include "motion.h"

Buzzer buzzer(BUZZER_PIN, BACKLIT_LED_PIN);

class Indicators
{
public:
    void begin()
    {
        setToneChannel(2);
        pinMode(INDICATOR_PIN, OUTPUT);
    }

    void customBlink_iter(int ontime, int offtime, int iterations)
    {
        for (int i = 0; i < iterations; i++)
        {
            digitalWrite(INDICATOR_PIN, HIGH);
            delay(ontime);
            digitalWrite(INDICATOR_PIN, LOW);
            delay(offtime);
        }
    }

    void customBlink_time(int ontime, int offtime, int duration)
    {
        int iterations = duration / (ontime + offtime);
        int mismatch = duration - iterations * (ontime + offtime);
        for (int i =0; i < iterations; i++)
        {
            digitalWrite(INDICATOR_PIN, HIGH);
            delay(ontime);
            digitalWrite(INDICATOR_PIN, LOW);
            delay(offtime);
        }
        delay(mismatch);
    }
    void indicateState(int state)
    {
        switch (state)
        {
        case 0:
            customBlink_iter(500, 500, 4);
            break;
        case 1:
            customBlink_time(200, 200, 2000);
            break;
        case 10:
            customBlink_time(150, 350, 8000);
            break;
        default:
            customBlink_time(150, 0, 150);
            break;
        }
    }

    void batteryLowIndicator(){
        if (analog.batteryRead() < 7.3){
            buzzer.begin(10);

            buzzer.sound(NOTE_D4, 400);
            buzzer.sound(0, 300);
            buzzer.sound(NOTE_CS4, 400);
            buzzer.sound(0, 300);
            buzzer.sound(NOTE_B3, 400);
            buzzer.sound(0, 300);

            buzzer.end(10);
        }
    }

    void backToBack()
    {   
        motion.stop();
        motion.disable_drive();

        buzzer.begin(10);

        buzzer.sound(NOTE_E4, 350);
        buzzer.sound(0, 350);
        buzzer.sound(NOTE_D4, 200);
        buzzer.sound(NOTE_D4, 200);
        buzzer.sound(NOTE_D4, 200);
        buzzer.sound(0, 250);

        buzzer.sound(NOTE_CS4, 200);
        buzzer.sound(NOTE_CS4, 200);
        buzzer.sound(NOTE_CS4, 200);
        buzzer.sound(0, 250);

        buzzer.sound(NOTE_G4, 125);
        buzzer.sound(NOTE_E4, 125);
        buzzer.sound(NOTE_D4, 125);
        buzzer.sound(NOTE_B3, 250);
        buzzer.sound(NOTE_A3, 125);
        buzzer.sound(NOTE_G3, 125);

        buzzer.sound(NOTE_E4, 350);
        buzzer.sound(0, 350);
        buzzer.sound(NOTE_D4, 200);
        buzzer.sound(NOTE_D4, 200);
        buzzer.sound(NOTE_D4, 200);
        buzzer.sound(0, 250);

        buzzer.sound(NOTE_CS4, 250);
        buzzer.sound(NOTE_CS4, 250);
        buzzer.sound(NOTE_CS4, 250);
        buzzer.sound(0, 250);

        buzzer.sound(NOTE_B3, 125);
        buzzer.sound(NOTE_G3, 125);
        buzzer.sound(NOTE_B3, 125);
        buzzer.sound(NOTE_A3, 250);
        buzzer.sound(NOTE_B3, 125);
        buzzer.sound(NOTE_AS3, 125);
        buzzer.sound(NOTE_B3, 125);
        buzzer.sound(NOTE_B3, 125);

        buzzer.end(10);

        motion.reset_drive_system();
    }
};

#endif // INDICATORS_H