/**
 * @file sensors.hpp
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The code related to sensors used by SmartPot.
 * @copyright Copyright (c) 2021
 */
#pragma once

#include "Comms.hpp"
#include "Display.hpp"

// The sensors are not always on. Before we read the values, we turn them on and
// wait for the `SENSOR_READ_DELAY` to read the data
#define SENSOR_READ_DELAY 1000

// The sensors can do multiple reads and average the values. They will wait the
// `SENSOR_READ_DELAY` between the consecutive reads
#define SENSOR_READ_COUNT 3

#define MIN_WATER_LVL 100
#define MAX_WATER_LVL 600
#define MIN_SOIL_LVL 1000
#define MAX_SOIL_LVL 300
#define MIN_LIGHT_LVL 150
#define MAX_LIGHT_LVL 1000

namespace SmartPot {
    namespace DefaultValues {
        const uint32_t read_soil_pin = A0;
        const uint32_t read_light_pin = A2;
        const uint32_t read_water_pin = A1;

        const uint32_t enable_pin = 7;
    }    // namespace DefaultValues

    class Sensors {
       private:
        uint32_t en_pin = DefaultValues::enable_pin;

        uint32_t soil_in = DefaultValues::read_soil_pin;
        uint32_t light_in = DefaultValues::read_light_pin;
        uint32_t water_in = DefaultValues::read_water_pin;

        Display* display;
        Comms* comms;

        /**
         * @brief Generic function that reads data from a sensor.
         * @param in The output pin (analog data input to the board)
         * @return int The value read from the sensor
         */
        int sensor_read(uint32_t in) {
            digitalWrite(en_pin, HIGH);

            int avg = 0;
            for (int i = 0; i < SENSOR_READ_COUNT; ++i) {
                int val = 0;
                delay(SENSOR_READ_DELAY);
                val = analogRead(in);
                avg = avg + val;
            }

            digitalWrite(en_pin, LOW);
            avg = avg / SENSOR_READ_COUNT;
            return avg;
        }

       public:
        typedef struct {
            int water;
            int soil;
            int light;
        } SensorsData;

        Sensors() {}

        /**
         * @brief Initialise the soil (moisture), water level and light sensors,
         * using the default pins.
         * @param d Pointer to a display object
         * @param c Pointer to a communications object
         */
        Sensors(Display* d, Comms* c) : display(d), comms(c) {
            pinMode(en_pin, OUTPUT);
        };

        /**
         * @brief Copy constructor for the sensors
         * @param other The other sensors object
         */
        Sensors(const Sensors& other) {
            this->display = other.display;
            this->comms = other.comms;
            pinMode(en_pin, OUTPUT);
        }

        /**
         * @brief Copy assignment operator for the sensor object
         * @param other The other sensor object
         * @return Sensors& The new sensors object
         */
        Sensors& operator=(const Sensors& other) {
            this->display = other.display;
            this->comms = other.comms;
            pinMode(en_pin, OUTPUT);
        };

        /**
         * @brief Read data from all the sensors
         * @return SensorsData The data
         */
        SensorsData read_all() {
            int water, soil, light;
            water = read_water();
            soil = read_soil();
            light = read_light();
            return {water, soil, light};
        }

        /**
         * @brief Get the water level from the water level sensor
         * @return int The "fill percent" (0-100)
         */
        int read_water() {
            int val = sensor_read(water_in);
            return map(val, MIN_WATER_LVL, MAX_WATER_LVL, 0, 100);
        }

        /**
         * @brief Get the moisture level of the soil, using the moisture
         * sensor
         * @return int The moisture level of the soil (0-100)
         */
        int read_soil() {
            int val = sensor_read(soil_in);
            return map(val, MIN_SOIL_LVL, MAX_SOIL_LVL, 0, 100);
        }

        /**
         * @brief Get the luminosity level, using the LDR sensor
         * @return int The luminosity level (0-100)
         */
        int read_light() {
            int val = sensor_read(light_in);
            return map(val, MIN_LIGHT_LVL, MAX_LIGHT_LVL, 0, 100);
        }
    };
}    // namespace SmartPot