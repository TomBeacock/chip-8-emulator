#include "display.h"

#include <iostream>
#include <algorithm>

namespace Chip8
{
    Display::Display(uint8_t width, uint8_t height)
        : width(width),
          height(height),
          display(width * height, false)
    {
        for (size_t y = 0; y < height; y++) {
            std::cout << std::endl;
        }
    }

    void Display::refresh()
    {
        // Draw
        for (size_t y = 0; y < height; y++) {
            for (size_t x = 0; x < width; x++) {
                std::cout << (display[y * width + x] ? '#' : '.');
            }
            std::cout << std::endl;
        }
    }

    void Display::drawSprite(uint8_t *src, uint8_t n, uint8_t x, uint8_t y)
    {
        // Wrap coord to screen
        x &= width - 1;
        y &= height - 1;

        // Iterate pixels
        for (size_t i = 0, dy = y; dy < std::min((uint8_t)(y + n), height); i++, dy++) {
            uint8_t byte = src[i];
            uint8_t mask = 0x80;
            for (size_t dx = x; dx < std::min((uint8_t)(x + 8), width); dx++, mask >>= 1) {
                // Flip pixel if on
                if ((byte & mask) > 0) {
                    display[dy * width + dx] = !display[dy * width + dx];
                }
            }
        }
    }

} // namespace Chip8