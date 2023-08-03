#pragma once

#include <cstdint>

namespace Chip8
{
    class Display;

    class Chip
    {
      public:
        Chip(Display *display);

      private:
        uint8_t memory[4096];
        Display *display;
    };
} // namespace Chip8