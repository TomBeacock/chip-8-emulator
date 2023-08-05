#pragma once

#include <cstdint>
#include <unordered_map>
#include <optional>

namespace Chip8
{
    class Chip;

    class IO
    {
      public:
        IO();

        bool isKeyDown(uint8_t n);

        void keyPressed(int scancode);
        void keyReleased(int scancode);
        std::optional<uint8_t> getReleasedKey();

        void reset();

      private:
        int keyMap[16];
        bool keyPressedStates[16];
        uint8_t lastReleased;
    };
} // namespace Chip8