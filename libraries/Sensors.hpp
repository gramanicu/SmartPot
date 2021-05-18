/**
 * @file sensors.hpp
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The code related to sensors used by SmartPot.
 * @copyright Copyright (c) 2021
 */

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

namespace SmartPot {
    namespace DefaultValues {
        const uint32_t read_soil_pin = A0;
        const uint32_t read_light_pin = A2;
        const uint32_t read_water_pin = A1;

        const uint32_t enable_soil_pin = 8;
        const uint32_t enable_light_pin = 9;
        const uint32_t enable_water_pin = 7;
    }    // namespace DefaultValues

    class Sensors {
       private:
        uint32_t soil_en = DefaultValues::enable_soil_pin;
        uint32_t light_en = DefaultValues::enable_light_pin;
        uint32_t water_en = DefaultValues::enable_water_pin;

        uint32_t soil_in = DefaultValues::read_soil_pin;
        uint32_t light_in = DefaultValues::read_light_pin;
        uint32_t water_in = DefaultValues::read_water_pin;

        /**
         * @brief Generic function that reads data from a sensor.
         * @param en The enable pin of the sensor (power pin)
         * @param in The output pin (analog data input to the board)
         * @return int The value read from the sensor
         */
        int sensor_read(uint32_t en, uint32_t in) {
            digitalWrite(en, HIGH);

            int avg = 0;
            for (int i = 0; i < SENSOR_READ_COUNT; ++i) {
                int val = 0;
                delay(SENSOR_READ_DELAY);
                val = analogRead(in);
                avg = avg + val;
            }

            digitalWrite(en, LOW);
            avg = avg / SENSOR_READ_COUNT;
            return avg;
        }

       public:
        typedef struct {
            int water;
            int soil;
            int light;
        } SensorsData;

        /**
         * @brief Initialise the soil (moisture), water level and light sensors,
         * using the default pins.
         */
        Sensors() {
            pinMode(water_en, OUTPUT);
            pinMode(soil_en, OUTPUT);
            pinMode(light_en, OUTPUT);
        };

        /**
         * @brief Initialise the moisture, water level and light sensors, using
         * specific pins.
         * @param se Soil (moisture) Enable Pin (digital out)
         * @param le Light Enable Pin (digital out)
         * @param we Water Enable Pin (digital out)
         * @param si Soil (moisture) Input (analog in)
         * @param li Light Input (analog in)
         * @param wi Water Input (analog in)
         */
        Sensors(uint32_t se, uint32_t le, uint32_t we, uint32_t si, uint32_t li,
                uint32_t wi)
            : soil_en(se),
              light_en(le),
              water_en(we),
              soil_in(si),
              light_in(li),
              water_in(wi) {
            pinMode(water_en, OUTPUT);
            pinMode(soil_en, OUTPUT);
            pinMode(light_en, OUTPUT);
        };

        /**
         * @brief Copy constructor for the sensors
         * @param other The other sensors object
         */
        Sensors(const Sensors& other)
            : soil_en(other.soil_en),
              light_en(other.light_en),
              water_en(other.water_en),
              soil_in(other.soil_in),
              light_in(other.light_in),
              water_in(other.water_in) {
            pinMode(water_en, OUTPUT);
            pinMode(soil_en, OUTPUT);
            pinMode(light_en, OUTPUT);
        };

        /**
         * @brief Copy assignment operator for the sensor object
         * @param other The other sensor object
         * @return Sensors& The new sensors object
         */
        Sensors& operator=(const Sensors& other) {
            this->soil_en = other.soil_en;
            this->light_en = other.light_en;
            this->water_en = other.water_en;
            this->soil_in = other.soil_in;
            this->light_in = other.light_in;
            this->water_in = other.water_in;
            pinMode(water_en, OUTPUT);
            pinMode(soil_en, OUTPUT);
            pinMode(light_en, OUTPUT);
        };

        SensorsData read_all() {
            int water, soil, light;
            water = read_water();
            soil = read_soil();
            light = read_light();
            return {water, soil, light};
        }

        int read_water() {
            int val = sensor_read(water_en, water_in);
            return map(val, MIN_WATER_LVL, MAX_WATER_LVL, 0, 100);
        }
        int read_soil() {
            int val = sensor_read(soil_en, soil_in);
            return map(val, MIN_SOIL_LVL, MAX_SOIL_LVL, 0, 100);
        }
        int read_light() { return sensor_read(light_en, light_in); }
    };
}    // namespace SmartPot