/**
 * @file SmartPot.ino
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The main code used by the SmartPot
 * @copyright Copyright (c) 2021
 */

#include "libraries/Display.hpp"
#include "libraries/Pump.hpp"
#include "libraries/Sensors.hpp"
#include "libraries/Serial.hpp"

using namespace SmartPot;

Display display;
Pump pump;
Sensors sensors;
// Serial serial;

void setup() {
    display = Display();
    pump = Pump();
    sensors = Sensors();
}

void loop() { display.update(); }
