/**
 * @file comms.hpp
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief Code related to serial communications used by SmartPot.
 * @copyright Copyright (c) 2021
 */
#pragma once

namespace SmartPot {
    namespace DefaultValues {
        const uint32_t baud_rate = 9600;
    }    // namespace DefaultValues

    class Comms {
       private:
        uint32_t baud_rate = DefaultValues::baud_rate;

       public:
        /**
         * @brief Initialise the serial communications, using the default baud
         * rate
         */
        Comms() { Serial.begin(baud_rate); }

        /**
         * @brief Copy constructor for the comms
         * @param other The other comms object
         */
        Comms(const Comms& other) { Serial.begin(baud_rate); }

        /**
         * @brief Copy assignment operator for the comms object
         * @param other The other comms object
         * @return Comms& The new comms object
         */
        Comms& operator=(const Comms& other) { Serial.begin(baud_rate); }

        /**
         * @brief Send data over the serial "link"
         * @param data The data to send
         */
        void print(const char* data) { Serial.println(data); }
    };

}    // namespace SmartPot