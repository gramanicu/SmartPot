/**
 * Copyright Grama Nicolae, 2020
 */

#include "Music.hpp"

#define SECONDS(t) (t * 1000)
#define MINUTES(t) (SECONDS(t) * 60)
#define HOURS(t) (MINUTES(t) * 60)

/**
 * Here it is stored the time between each state check (check if
 * there is enough water in the reservoir, moisture level is high
 * enough, etc.). 
 */
#define GENERAL_TIMEOUT HOURS(6)

const int alarmPin = 13;

// Returns the water level (1-100)
int waterLevel() {
}

// Returns the moisture level of the soil (1-100)
int moistureLevel() {
}

// Checks if the reservoir needs a refill
bool needRefill() {
    // Check the water level in the reservoir.
    // If it is under a treshold, call "lowLevelAlarm" function
}

// Checks if the pot needs water
bool needWatering() {
    // Check the moisture level. If it is under
    // a treshold, call "waterPlant" function
}

// Add water to the pot
void waterPlant() {
    // Turn the valve/pump on
    // After a set timeout, close it
}

// Notify (using sounds), that the water level in the
// reservoir is too low
void lowLevelAlarm() {
    tone(alarmPin, A1#, 20);
}

// Set the pins 
void setup() {
    // Assign different pins
}

// Check the state of the system in a loop
void loop() {
    // If the water level is high enough, and the
    // moisture level is low, water the pot. This
    // will happen at a specified timeout (to increase
    // the lifetime of the sensors - they can degrade 
    // through electrolysis)
    if(!needRefill() && needWatering()) {
        waterPlant();
    }

    delay(GENERAL_TIMEOUT);
}
