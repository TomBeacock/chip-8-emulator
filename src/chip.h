#pragma once

#include <cstdint>
#include <stack>
#include <vector>

namespace Chip8
{
    class Display;
    class IO;

    class Chip
    {
      public:
        Chip(Display *display, IO *io);

        void loadProgram(const char *file);
        void decrementTimers();
        void runNext();

      private:
        uint16_t fetch();
        void decodeExecute(uint16_t instruction);

      private:
        uint8_t memory[4096];
        uint16_t pc = 0x200;
        uint16_t index = 0;
        uint8_t registers[16] = {0};
        std::stack<uint16_t> stack;
        uint8_t soundTimer = 0, delayTimer = 0;
        Display *display;
        IO *io;
    };
} // namespace Chip8