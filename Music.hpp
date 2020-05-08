// Copyright Grama Nicolae, 2020

// This library is used to play different songs using the "tone()" function
// a Arduino. Firstly, the output pin must be set. Then, using a mathematical
// found and the frequency of the D0 note, every other notes frequency is
// determined and can be played

#include <Arduino.h>

#ifndef Music_hpp
#define Music_hpp

namespace Music {
    #define C0 16.35
    
    struct Note {
        String name;
        int duration;
    };
    
    static int output_pin;

    // Each octave has 12 semitones. This function will return the number
    // of semitones the specified note is away from "C0". Will return 0
    // if the noteName is "_" (which is considered a pause)
    static int get_note_num(String noteName) {
        if (noteName == "_") {
            return 0;
        }

        char note = noteName.charAt(0);
        int octave = noteName.charAt(1) - '0';
        char modifier = ' ';
        if(noteName.length() == 3) {
            modifier = noteName.charAt(2);
        }

        int num = octave * 12;
        switch (note) {
            case 'A': {
                num += 9;
            } break;
            case 'B': {
                num += 11;
            } break;
            case 'C': {
                num += 0;
            } break;
            case 'D': {
                num += 2;
            } break;
            case 'E': {
                num += 4;
            } break;
            case 'F': {
                num += 5;
            } break;
            case 'G': {
                num += 7;
            } break;
            default:
                return 0;
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

    // Set the output pin for the sounds
    static void set_output(const int pin) { output_pin = pin; }

    // Returns the pin on which the buzzer is connected
    static int get_pin() { return output_pin; }

    // Returns the frequency of the note.
    static float get_frequency(String noteName) { 
        return C0 * pow (2, float(get_note_num(noteName)) / 12.0f);
    }

    // Play a note with a specified duration (in ms)
    static void play_note(String noteName, int duration) {
        int freq = get_frequency(noteName);
        if(freq != 0) {
            tone(output_pin, freq);
            delay(duration);
        }
        noTone(output_pin);
    }

    // This will play three beeps as an alarm
    static void play_alarm() {
        Note melody[] = {
            {"C7", 1000},
            {"_", 100},
            {"C7", 1000},
            {"_", 100},
            {"C7", 1000}
        };

        for(int i = 0; i < 5; ++i) {
            Note note = melody[i];
            play_note(note.name, note.duration);
        }
    }
};

#endif
