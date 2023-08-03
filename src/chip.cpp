#include "chip.h"

#include "display.h"

#include <algorithm>
#include <iterator>

namespace Chip8
{
    Chip::Chip(Display *display) : display(display)
    {
        uint8_t font[16 * 5] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };
        std::copy(std::begin(font), std::end(font), std::begin(memory) + 0x50);

        for (size_t i = 0; i < 10; i++) {
            display->drawSprite(&memory[0x50 + i * 5], 5, i * 6, 0);
        }
        for (size_t i = 0; i < 6; i++) {
            display->drawSprite(&memory[0x50 + (i + 10) * 5], 5, i * 6, 6);
        }
    }
} // namespace Chip8