/**
 * @file pump.hpp
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief  The code related to the water pump used by SmartPot.
 * @copyright Copyright (c) 2021
 */

#define MIN_POWER_ABS 100
#define MAX_POWER_ABS 255

namespace SmartPot {
    namespace DefaultValues {
        // Sets the power of the pump. Must be a PWM pin
        const uint32_t set_motor_pin = 10;
        const uint32_t enable_motor_pin = 6;
    }    // namespace DefaultValues

    class Pump {
       private:
        uint32_t power_pin = DefaultValues::set_motor_pin;
        uint32_t enable_pin = DefaultValues::enable_motor_pin;

       public:
        /**
         * @brief Initialise the water pump object, using
         * the default pins
         */
        Pump() {
            pinMode(power_pin, OUTPUT);
            pinMode(enable_pin, OUTPUT);
        };

        /**
         * @brief Initialise the water pump object, using
         * specific pins.
         * @param sp The power control for the pump
         * @param en The enable pin for the pump
         */
        Pump(uint32_t sp, uint32_t en) : power_pin(sp), enable_pin(en) {
            pinMode(power_pin, OUTPUT);
            pinMode(enable_pin, OUTPUT);
        };

        /**
         * @brief Copy constructor for the pump object
         * @param other The other pump object
         */
        Pump(const Pump& other)
            : power_pin(other.power_pin), enable_pin(other.enable_pin) {
            pinMode(power_pin, OUTPUT);
            pinMode(enable_pin, OUTPUT);
        };

        /**
         * @brief Copy assignment operator for the pump object
         * @param other The other pump object
         * @return Pump& The new pump object
         */
        Pump& operator=(const Pump& other) {
            this->power_pin = other.power_pin;
            this->enable_pin = other.enable_pin;
            pinMode(power_pin, OUTPUT);
            pinMode(enable_pin, OUTPUT);
        };

        /**
         * @brief Run the pump, at a specific power (percentage), for the
         * specified amount of time (in ms)
         * @param power The percent of the maximum power
         * @param duration The time to run the pump for (in ms)
         */
        void run(uint32_t power, uint32_t duration) {
            start(power);
            delay(duration);
            stop();
        }

        /**
         * @brief Start the pump and set the power (percentage)
         * @param power The percent of the maximum power
         */
        void start(uint32_t power) {
            digitalWrite(enable_pin, HIGH);
            analogWrite(power_pin,
                        map(power, 0, 100, MIN_POWER_ABS, MAX_POWER_ABS));
        }

        /**
         * @brief Stop the pump
         */
        void stop() {
            analogWrite(power_pin, 0);
            digitalWrite(enable_pin, LOW);
        }
    };
}    // namespace SmartPot
