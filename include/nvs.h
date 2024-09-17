#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include "maze.h"

class NVS;

extern NVS nvs;

class NVS
{
public:
    Preferences preferences;

    void saveArrays()
    {
        preferences.begin("maze_data", false);

        size_t written = preferences.putBytes("m_cost", (uint8_t *)maze.m_cost, sizeof(maze.m_cost));
        Serial.printf("Saved %d bytes of m_cost\n", written);

        written = preferences.putBytes("m_walls", (uint8_t *)maze.m_walls, sizeof(maze.m_walls));
        Serial.printf("Saved %d bytes of m_walls\n", written);
        
        preferences.end();
    }

    void loadArrays()
    {
        preferences.begin("maze_data", true);

        // Load m_cost array
        preferences.getBytes("m_cost", (uint8_t *)maze.m_cost, sizeof(maze.m_cost));

        preferences.getBytes("m_walls", (uint8_t *)maze.m_walls, sizeof(maze.m_walls));
        
        preferences.end();
    }
};