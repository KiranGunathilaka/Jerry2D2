#pragma once

#include <Arduino.h>
#include <config.h>

class Analog;

extern Analog analog;

class Analog
{

public:

    int switchRead()
    {
        int analogValue = analogRead(SWITCH_PIN); // Read the analog value (0 to 4095)
        if(analogValue > 3800){
            return 1;
        }
        else if(analogValue > 2800 ){
            return 2;
        }else if(analogValue > 1800 ){
            return 3;
        }else if(analogValue > 800 ){
            return 4;
        }
        return 0;
    }

    void batteryRead(){
        int analogValue = analogRead(BATTERY_PIN);
        battery_voltage = analogValue * (3.3/4095) * 3 + 0.26;

    }
    inline float batteryVoltage(){
        return battery_voltage;
    }
    
private:
    float battery_voltage;
};