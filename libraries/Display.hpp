/**
 * @file display.hpp
 * @author Grama Nicolae (gramanicu@gmail.com)
 * @brief The code related to lcd display used by SmartPot.
 * @copyright Copyright (c) 2021
 */
#pragma once

#include <LiquidCrystal.h>

// A 2x16 LCD - model NDS 1602A
#define LINE_SIZE 16
#define LINE_COUNT 2
#define MAX_CHARS (LINE_SIZE * 8)
#define SCROLL_DELAY 250

namespace SmartPot {
    namespace DefaultValues {
        const uint32_t lcd_d4 = 5;
        const uint32_t lcd_d5 = 4;
        const uint32_t lcd_d6 = 3;
        const uint32_t lcd_d7 = 2;
        const uint32_t lcd_rs = 12;    // LCD - Register Select Pin
        const uint32_t lcd_en = 11;    // LCD - Enable Pin
    }                                  // namespace DefaultValues

    class Display {
       private:
        // LCD data pins
        LiquidCrystal* lcd;
        uint32_t rs_pin = DefaultValues::lcd_rs;
        uint32_t en_pin = DefaultValues::lcd_en;
        uint32_t d4_pin = DefaultValues::lcd_d4;
        uint32_t d5_pin = DefaultValues::lcd_d5;
        uint32_t d6_pin = DefaultValues::lcd_d6;
        uint32_t d7_pin = DefaultValues::lcd_d7;

        char line_buffer_1[MAX_CHARS];
        char line_buffer_2[MAX_CHARS];

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
         * @brief Copy constructor for the lcd display object
         * @param other The other display object
         */
        Display(const Display& other) {
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
            lcd = new LiquidCrystal(rs_pin, en_pin, d4_pin, d5_pin, d6_pin,
                                    d7_pin);
            lcd->begin(LINE_SIZE, LINE_COUNT);
        };

        /**
         * @brief Display a line on the LCD.
         * @param line The string to print
         * @param which Which line to print to (default 0)
         */
        void print(const char* line, uint8_t which = 0) {
            if (which == 0) {
                memset(line_buffer_1, '\0', MAX_CHARS * sizeof(char));
                strncpy(line_buffer_1, line, min(MAX_CHARS, strlen(line)));
            } else if (which == 1) {
                memset(line_buffer_2, '\0', MAX_CHARS * sizeof(char));
                strncpy(line_buffer_2, line, min(MAX_CHARS, strlen(line)));
            }
        }

        /**
         * @brief Print 2 strings on the LCD.
         * @param line1 The first line (0 line)
         * @param line2 The second line (1 line)
         */
        void print(const char* line1, const char* line2) {
            memset(line_buffer_1, '\0', MAX_CHARS * sizeof(char));
            memset(line_buffer_2, '\0', MAX_CHARS * sizeof(char));
            strncpy(line_buffer_1, line1, min(MAX_CHARS, strlen(line1)));
            strncpy(line_buffer_2, line2, min(MAX_CHARS, strlen(line2)));
        }

        void clear_buffer(const uint32_t which) {
            if (which == 0) {
                memset(line_buffer_1, '\0', MAX_CHARS * sizeof(char));
                memset(line_buffer_1, ' ', LINE_SIZE * sizeof(char));
                line_buffer_1[LINE_SIZE + 1] = '\0';
            } else if (which == 1) {
                memset(line_buffer_2, '\0', MAX_CHARS * sizeof(char));
                memset(line_buffer_2, ' ', LINE_SIZE * sizeof(char));
                line_buffer_2[LINE_SIZE + 1] = '\0';
            }
        }

        /**
         * @brief Display the buffered data on the LCD. Should be run inside the
         * `loop` arduino function
         */
        void update() {
            // If the line that must be printed is longer than "LINE_SIZE", the
            // text must be scrolled
            uint32_t b1_start = 0, b2_start = 0;
            bool finished = false;

            bool fast = strlen(line_buffer_2) != 0 ? false : true;

            while (!finished) {
                String line1 = String(line_buffer_1)
                                   .substring(b1_start, b1_start + LINE_SIZE);
                String line2 = String(line_buffer_2)
                                   .substring(b2_start, b2_start + LINE_SIZE);
                finished = true;

                // Print line
                lcd->setCursor(0, 0);
                lcd->print(line1.c_str());
                lcd->setCursor(0, 1);
                lcd->print(line2.c_str());

                if (b1_start == 0 && b2_start == 0 && !fast) {
                    delay(SCROLL_DELAY * 8);
                }

                // Scroll unfinished lines
                if (strlen(line_buffer_1) - b1_start > LINE_SIZE) {
                    b1_start++;
                    finished = false;
                }

                if (strlen(line_buffer_2) - b2_start > LINE_SIZE) {
                    b2_start++;
                    finished = false;
                }

                if (!fast) { delay(SCROLL_DELAY); }
            }
            if (!fast) { delay(SCROLL_DELAY * 7); }
        }
    };
}    // namespace SmartPot
