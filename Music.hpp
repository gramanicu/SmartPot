// Copyright Grama Nicolae, 2020

// This library is used to play different songs using the "tone()" function
// a Arduino. Firstly, the output pin must be set. Then, using a mathematical
// found and the frequency of the A4 note, every other notes frequency is
// determined and can be played

//https://www.quora.com/Which-algorithm-can-be-used-to-go-from-frequency-data-to-a-musical-note

#ifndef Music_hpp
#define Music_hpp

class Music {
    private:
        #define DEFAULT_ROOT 440

        int root_note;
        int output_pin;

        static int get_frequency(char* note, int root = DEFAULT_ROOT) {
            
        }

    public:
        Music(const int out, const int root = DEFAULT_ROOT): root_note(root), output_pin(out) {};

};

#endif