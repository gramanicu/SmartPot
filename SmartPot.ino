/**
 * Copyright Grama Nicolae, 2020
 */

#include "Music.hpp"

#define SECONDS(t) (t * 1000L)
#define MINUTES(t) (SECONDS(t) * 60L)
#define HOURS(t) (MINUTES(t) * 60L)

/**
 * Here it is stored the time between each state check (check if
 * there is enough water in the reservoir, moisture level is high
 * enough, etc.). 
 */
#define GENERAL_TIMEOUT HOURS(6)
#define PUMP_SPEED 10               // 0 - 100%
#define PUMP_DURATION SECONDS(5)
#define MOISTURE_TRESHOLD 30       // 0 - 100%
#define WATER_LEVEL_TRESHOLD 10     // 0 - 100%
#define SENSOR_READ_DELAY SECONDS(1)

const int alarmPin = 13;          // Powers the buzzer
const int pumpSpeedPin = 5;       // Alters the speed of the pumo
const int pumpEnablePin = 4;      // Powers the H-Bridge and enables the motor
const int waterEnablePin = 3;     // Powers the water level sensor
const int moistureEnablePin = 2;  // Powers the moisture sensor

const int waterSensor = A1;    // Returns analog data from the water level sensor
const int moistureSensor = A0; // Returns analog data from the moisture sensor

// Set the pins 
void setup() {
    // Assign different pins
    Music::set_output(alarmPin);

    pinMode(pumpSpeedPin, OUTPUT);
    pinMode(pumpEnablePin, OUTPUT);
    pinMode(waterEnablePin, OUTPUT);
    pinMode(moistureEnablePin, OUTPUT);

    Serial.begin(9600);
    lowLevelAlarm();
}

// Returns the water level (1-100)
int waterLevel() {
    digitalWrite(waterEnablePin, HIGH);
    delay(SENSOR_READ_DELAY);
    int value = analogRead(waterSensor);
    value = map(value, 100, 600, 0, 100);
    digitalWrite(waterEnablePin, LOW);
    return value;
}

// Returns the moisture level of the soil (1-100)
int moistureLevel() {
    digitalWrite(moistureEnablePin, HIGH);
    delay(SENSOR_READ_DELAY);
    int value = analogRead(moistureSensor);
    value = map(value, 1000, 300, 0, 100);
    digitalWrite(moistureEnablePin, LOW);
    return value;
}

// Checks if the reservoir needs a refill
bool needRefill() {
    // Check the water level in the reservoir.
    // If it is under a treshold, call "lowLevelAlarm" function
    if(waterLevel() < WATER_LEVEL_TRESHOLD) {
        lowLevelAlarm();
        return true;
    }

    return false;
}

// Checks if the pot needs water
bool needWatering() {
    // Check the moisture level. If it is under
    // a treshold, call "waterPlant" function
    if(moistureLevel() < MOISTURE_TRESHOLD) {
        return true;
    }
    return false;
}

// Add water to the pot
// power - the power/flow of the pump (as percent)
// duration - how much time it will pump
void waterPlant(int power, long duration) {
    // Turn the valve/pump on
    // After a set timeout, close it
    digitalWrite(pumpEnablePin, HIGH);
    analogWrite(pumpSpeedPin, map(power, 0, 100, 100, 255));
    delay(duration);
    analogWrite(pumpSpeedPin, 0);
    digitalWrite(pumpEnablePin, LOW);
}

// Notify (using sounds), that the water level in the
// reservoir is too low
void lowLevelAlarm() {
    Music::play_alarm();
}

// Check the state of the system in a loop
void loop() {
    // If the water level is high enough, and the
    // moisture level is low, water the pot. This
    // will happen at a specified timeout (to increase
    // the lifetime of the sensors - they can degrade 
    // through electrolysis)
    if(Serial.available()) {
        String command = Serial.readString();
        command.replace("\n","");
        Serial.println("Received " + command); 
        if(command == "water_plant") {
            waterPlant(PUMP_SPEED, PUMP_DURATION);
        }
        if(command == "empty_reservoir") {
            waterPlant(100, SECONDS(10));
        }
        if(command == "water_level") {
            Serial.print("Water level is : ");
            Serial.print(waterLevel());
            Serial.println("%");
        }
        if(command == "moisture_level") {
            Serial.print("Moisture level is : ");
            Serial.print(moistureLevel());
            Serial.println("%");
        }
    }
    
    if(!needRefill() && needWatering()) {
        waterPlant(PUMP_SPEED, PUMP_DURATION);
    }

    delay(GENERAL_TIMEOUT);
//    delay(HOURS(0) + MIN/UTES(0) + SECONDS(5));
}
