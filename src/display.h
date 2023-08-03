#pragma once

#include <cstdint>
#include <vector>

namespace Chip8
{
    class Display
    {
      public:
        Display(uint8_t width, uint8_t height);

        void refresh();
        void drawSprite(uint8_t *src, uint8_t n, uint8_t x, uint8_t y);

      private:
        uint8_t width;
        uint8_t height;
        std::vector<bool> display;
    };
} // namespace Chip8