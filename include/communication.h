#pragma once

#include <WiFi.h>
#include <WiFiUdp.h>
#include "config.h"

class Communication;

WiFiUDP udp;

extern Communication communication;
class Communication {
 public:
   void begin(){
        Serial.begin(115200);
        if (WIFI_ENABLE == true){
            WiFi.begin(SSID, WIFI_PASSWORD);
        for (int i=1;i<3;i++){
            if(WiFi.status() != WL_CONNECTED){
               delay(1000);
               Serial.println("Connecting to WiFi...");
            }else {
                  Serial.println("WiFi connected");
                  Serial.println("IP address: " + WiFi.localIP().toString());
            }
        if(WiFi.status() != WL_CONNECTED){
            Serial.println("Connected without Wifi");
        }
        }

      }
    }
    void send(String data){
          Serial.println(String(data));
          udp.beginPacket(HOST_IP, HOST_PORT);
          udp.print(String(data));
          udp.endPacket();
    }

 private:
};