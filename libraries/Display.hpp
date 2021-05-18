/**
 * @file display.hpp
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The code related to lcd display used by SmartPot.
 * @copyright Copyright (c) 2021
 */

#include <LiquidCrystal.h>

// A 2x16 LCD - model NDS 1602A
#define LINE_SIZE 16
#define LINE_COUNT 2

namespace SmartPot {
    namespace DefaultValues {
        const uint32_t lcd_d4 = 5;
        const uint32_t lcd_d5 = 4;
        const uint32_t lcd_d6 = 3;
        const uint32_t lcd_d7 = 2;
        const uint32_t lcd_rs = 12;        // LCD - Register Select Pin
        const uint32_t lcd_enable = 11;    // LCD - Enable Pin
    }                                      // namespace DefaultValues

    class Display {
       private:
        // LCD data pins
        LiquidCrystal* lcd;
        uint32_t rs_pin = DefaultValues::lcd_rs;
        uint32_t en_pin = DefaultValues::lcd_enable;
        uint32_t d4_pin = DefaultValues::lcd_d4;
        uint32_t d5_pin = DefaultValues::lcd_d5;
        uint32_t d6_pin = DefaultValues::lcd_d6;
        uint32_t d7_pin = DefaultValues::lcd_d7;

        char line_buffer_1[LINE_SIZE];
        char line_buffer_2[LINE_SIZE];

       public:
        /**
         * @brief Initialise the lcd display, using
         * the default pins.
         */
        Display() {
            lcd = new LiquidCrystal(rs_pin, en_pin, d4_pin, d5_pin, d6_pin,
                                    d7_pin);
            lcd->begin(LINE_SIZE, LINE_COUNT);
        };

        /**
         * @brief Initialise the lcd display, using
         * specific pins.
         * @param rs The reset LCD pin
         * @param en The enable LCD pin
         * @param d4 The D4 LCD pin
         * @param d5 The D5 LCD pin
         * @param d6 The D6 LCD pin
         * @param d7 The D7 LCD pin
         */
        Display(uint32_t rs, uint32_t en, uint32_t d4, uint32_t d5, uint32_t d6,
                uint32_t d7)
            : rs_pin(rs),
              en_pin(en),
              d4_pin(d4),
              d5_pin(d5),
              d6_pin(d6),
              d7_pin(d7) {
            lcd = new LiquidCrystal(rs_pin, en_pin, d4_pin, d5_pin, d6_pin,
                                    d7_pin);
            lcd->begin(LINE_SIZE, LINE_COUNT);
        };

        /**
         * @brief Copy constructor for the lcd display object
         * @param other The other display object
         */
        Display(const Display& other)
            : rs_pin(other.rs_pin),
              en_pin(other.en_pin),
              d4_pin(other.d4_pin),
              d5_pin(other.d5_pin),
              d6_pin(other.d6_pin),
              d7_pin(other.d7_pin) {
            lcd = new LiquidCrystal(rs_pin, en_pin, d4_pin, d5_pin, d6_pin,
                                    d7_pin);
            lcd->begin(LINE_SIZE, LINE_COUNT);
        };

        /**
         * @brief Copy assignment operator for the display object
         * @param other The other display object
         * @return Display& The new display object
         */
        Display& operator=(const Display& other) {
            this->rs_pin = other.rs_pin;
            this->en_pin = other.en_pin;
            this->d4_pin = other.d4_pin;
            this->d5_pin = other.d5_pin;
            this->d6_pin = other.d6_pin;
            this->d7_pin = other.d7_pin;
            lcd = new LiquidCrystal(rs_pin, en_pin, d4_pin, d5_pin, d6_pin,
                                    d7_pin);
            lcd->begin(LINE_SIZE, LINE_COUNT);
        };

        void print(const char* line, uint8_t which = 0) {
            if (which == 0) {
                memset(line_buffer_2, ' ', LINE_SIZE * sizeof(char));
                strncpy(line_buffer_1, line, min(LINE_SIZE, strlen(line)));
                line_buffer_1[21] = '\0';
            } else if (which == 1) {
                memset(line_buffer_2, ' ', LINE_SIZE * sizeof(char));
                strncpy(line_buffer_2, line, min(LINE_SIZE, strlen(line)));
                line_buffer_2[21] = '\0';
            }
        }

        void print(const char* line1, const char* line2) {
            memset(line_buffer_1, ' ', LINE_SIZE * sizeof(char));
            memset(line_buffer_2, ' ', LINE_SIZE * sizeof(char));
            strncpy(line_buffer_1, line1, min(LINE_SIZE, strlen(line1)));
            strncpy(line_buffer_2, line2, min(LINE_SIZE, strlen(line2)));
            line_buffer_1[21] = '\0';
            line_buffer_2[21] = '\0';
        }

        /**
         * @brief Display the buffered data on the LCD. Should be run inside the
         * `loop` arduino function
         */
        void update() {
            lcd->setCursor(0, 0);
            lcd->print(line_buffer_1);
            lcd->setCursor(0, 1);
            lcd->print(line_buffer_2);
        }
    };
}    // namespace SmartPot
