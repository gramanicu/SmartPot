#include <iostream>

int main() {
    std::string noteName;
    std::cin >> noteName;
        char note = noteName.at(0);
        int octave = noteName.at(1) - '0';
        char modifier;
        if(noteName.length() == 3) {
            modifier = noteName.at(2);
        }
    
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

    std::cout << num <<  "\n";
}