#pragma once
#include <esp_now.h>
#include <WiFi.h>
#include <config.h>

class Reporting;

extern Reporting reporter;


class Reporting
{
public:
    typedef struct sendData
    {
        int intData;
        float floatData;
        String stringData;
    } sendData;

    typedef struct receiveData
    {
        int intCmd;
        float floatCmd;
        char stringCmd[20];
    } receiveData;

    receiveData command;
    esp_now_peer_info_t peerInfo;
    sendData transmitData;
    static Reporting *instance;

    static void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
    {
        Serial.print("\r\nLast Packet Send Status:\t");
        Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Failed");
    }

    static void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
    {
        if (instance)
        {
            memcpy(&instance->command, incomingData, sizeof(instance->command));
            Serial.print("Int: ");
            Serial.print(instance->command.intCmd);
            Serial.print("  Float: ");
            Serial.print(instance->command.floatCmd);
            Serial.print("  String: ");
            Serial.println(instance->command.stringCmd);
        }
    }

    void begin()
    {
        instance = this; // Set the instance pointer

        Serial.begin(115200);
        WiFi.mode(WIFI_STA);

        // Init ESP-NOW
        if (esp_now_init() != ESP_OK)
        {
            Serial.println("Error initializing ESP-NOW");
            return;
        }

        // Register for Send CB to get the status of Transmitted packet
        esp_now_register_send_cb(OnDataSent);

        // Register peer
        memcpy(peerInfo.peer_addr, broadcastAddress, 6);
        peerInfo.channel = 0;
        peerInfo.encrypt = false;

        // Add peer
        if (esp_now_add_peer(&peerInfo) != ESP_OK)
        {
            Serial.println("Failed to add peer");
            return;
        }

        // Receiving code
        esp_now_register_recv_cb(OnDataRecv);
    }

    void send(int intdata = 0, float floatdata = 0.0, String stringdata = "")
    {
        transmitData.intData = intdata;
        transmitData.floatData = floatdata;
        transmitData.stringData = stringdata;
        // Send message via ESP-NOW
        esp_now_send(broadcastAddress, (uint8_t *)&transmitData, sizeof(transmitData));
    }
};