// Copyright Grama Nicolae, 2020

// This library is used to play different songs using the "tone()" function
// a Arduino. Firstly, the output pin must be set. Then, using a mathematical
// found and the frequency of the A4 note, every other notes frequency is
// determined and can be played

// https://www.quora.com/Which-algorithm-can-be-used-to-go-from-frequency-data-to-a-musical-note

#include <Arduino.h>

#ifndef Music_hpp
#define Music_hpp

class Music {
   private:
#define C0 440

    static int output_pin;
    Music(){};  // Make the class "static"

    static int get_note_num(String noteName) {
        char note = noteName.charAt(0);
        int octave = noteName.charAt(1) - '0';
        char modifier = noteName.charAt(2);

        int num = octave * 12;
        switch (note) {
            case 'A': {
                num += 5;
            } break;
            case 'B': {
                num += 6;
            } break;
            case 'C': {
                num += 0;
            } break;
            case 'D': {
                num += 1;
            } break;
            case 'E': {
                num += 2;
            } break;
            case 'F': {
                num += 3;
            } break;
            case 'G': {
                num += 4;
            } break;
            default:
                return -1;
                break;
        }

        switch (modifier) {
            case '#': {
                num += 1;
            } break;
            case 'b': {
                num -= 1;
            } break;
            default: { num += 0; } break;
        }

        return num;
    }

   public:
    static void set_output(const int pin) { output_pin = pin; }

    static int get_pin() { return output_pin; }

    static int get_frequency(String noteName) { int frequency = C0; }
};

#endif