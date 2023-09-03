#ifndef COLOR_H
#define COLOR_H

#include <cstdint>

class Color {
public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    Color(): red(0), green(0), blue(0) {};
    Color(uint8_t r, uint8_t g, uint8_t b): red(r), green(g), blue(b) {};
};

#endif
