#include "chip.h"

#include "display.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <iostream>

#define KEY(n) ((keyPressedStates >> n) & 1)

namespace Chip8
{
    Chip::Chip(Display *display) : display(display)
    {
        // Load font
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
    }

    void Chip::runNext()
    {
        uint16_t instruction = fetch();
        decodeExecute(instruction);
    }

    uint16_t Chip::fetch()
    {
        uint16_t instruction = (memory[pc] << 8) | memory[pc + 1];
        pc += 2;
        return instruction;
    }

    void Chip::decodeExecute(uint16_t instruction)
    {
        // Extract instruction components
        uint8_t category = (instruction & 0xF000) >> 12;
        uint8_t x = (instruction & 0x0F00) >> 8;
        uint8_t y = (instruction & 0x00F0) >> 4;
        uint8_t n = instruction & 0x000F;
        uint8_t nn = instruction & 0x00FF;
        uint16_t nnn = instruction & 0x0FFF;

        // Decode & execute
        switch (category) {
            case 0x0: {
                switch (nnn) {
                    // Clear Screen
                    case 0x00E0: {
                        display->clear();
                        break;
                    }
                    // Subroutine return
                    case 0x00EE: {
                        pc = stack.top();
                        stack.pop();
                        break;
                    }
                }
                break;
            }
            // Jump
            case 0x1: {
                pc = nnn;
                break;
            }
            // Begin subroutine
            case 0x2: {
                stack.push(pc);
                pc = nnn;
                break;
            }
            // Skip if equal
            case 0x3: {
                uint8_t regX = registers[x];
                if (regX == nn) {
                    pc += 2;
                }
                break;
            }
            // Skip if not equal
            case 0x4: {
                uint8_t regX = registers[x];
                if (regX != nn) {
                    pc += 2;
                }
                break;
            }
            // Skip if registers equal
            case 0x5: {
                uint8_t regX = registers[x];
                uint8_t regY = registers[y];
                if (regX == regY) {
                    pc += 2;
                }
                break;
            }
            // Set register
            case 0x6: {
                registers[x] = nn;
                break;
            }
            // Add
            case 0x7: {
                registers[x] += nn;
                break;
            }
            // Arithmetic
            case 0x8: {
                switch (n) {
                    // Set
                    case 0x0: {
                        registers[x] = registers[y];
                        break;
                    }
                    // Binary OR
                    case 0x1: {
                        registers[x] |= registers[y];
                        break;
                    }
                    // Binary AND
                    case 0x2: {
                        registers[x] &= registers[y];
                        break;
                    }
                    // Binary XOR
                    case 0x3: {
                        registers[x] ^= registers[y];
                        break;
                    }
                    // Add
                    case 0x4: {
                        uint32_t val = registers[x] + registers[y];
                        registers[x] = val & 255;
                        registers[0xF] = val > 255;
                        break;
                    }
                    // Subtract
                    case 0x5: {
                        registers[x] -= registers[y];
                        registers[0xF] = registers[x] >= registers[y];
                        break;
                    }
                    // Right shift
                    case 0x6: {
                        registers[0xF] = registers[x] & 1;
                        registers[x] >>= 1;
                        break;
                    }
                    // Subtract
                    case 0x7: {
                        registers[x] = registers[y] - registers[x];
                        registers[0xF] = registers[y] >= registers[x];
                        break;
                    }
                    // Left shift
                    case 0xE: {
                        registers[0xF] = registers[x] >> 7;
                        registers[x] <<= 1;
                        break;
                    }
                }
                break;
            }
            // Skip if registers equal
            case 0x9: {
                uint8_t regX = registers[x];
                uint8_t regY = registers[y];
                if (regX == regY) {
                    pc += 2;
                }
                break;
            }
            // Set index
            case 0xA: {
                index = nnn;
                break;
            }
            // Jump with offset
            case 0xB: {
                pc = nnn + registers[0];
                break;
            }
            // Random
            case 0xC: {
                uint8_t r = rand();
                registers[x] = r & nn;
                break;
            }
            // Display
            case 0xD: {
                display->drawSprite(
                    memory + index, n, registers[x], registers[y]);
                break;
            }
            // Keys
            case 0xE: {
                switch (nn) {
                    // Key down
                    case 0x9E: {
                        if (KEY(registers[x])) {
                            pc += 2;
                        }
                        break;
                    }
                    // Key up
                    case 0xA1: {
                        if (KEY(registers[x])) {
                            pc += 2;
                        }
                        break;
                    }
                }
                break;
            }
            // General
            case 0xF: {
                switch (nn) {
                    // Read delay timer
                    case 0x07: {
                        registers[x] = delayTimer;
                        break;
                    }
                    // Set delay timer
                    case 0x15: {
                        delayTimer = registers[x];
                        break;
                    }
                    // Set sound timer
                    case 0x18: {
                        soundTimer = registers[x];
                        break;
                    }
                    // Add to index
                    case 0x1E: {
                        index += registers[x];
                        break;
                    }
                    // Get key
                    case 0x0A: {
                        if (keyReleasedStates == 0) {
                            pc -= 2;
                        } else {
                            uint16_t key = 0;
                            for (uint16_t temp = keyReleasedStates;
                                 (temp & 1) == 0; temp >>= 1) {
                                key++;
                            }
                            registers[x] = key;
                        }
                        break;
                    }
                    // Font character
                    case 0x29: {
                        index = 0x50 + registers[x] * 5;
                        break;
                    }
                    // Binary-code decimal conversion
                    case 0x33: {
                        uint8_t val = registers[x];
                        for (size_t i = 2; i >= 2; i--) {
                            uint8_t digit = val % 10;
                            memory[index + i] = digit;
                            val = (val - digit) / 10;
                        }
                        break;
                    }
                    // Store
                    case 0x55: {
                        for (uint8_t i = 0; i <= x; i++) {
                            memory[index + i] = registers[i];
                        }
                        break;
                    }
                    // Load
                    case 0x65: {
                        for (uint8_t i = 0; i <= x; i++) {
                            registers[i] = memory[index + i];
                        }
                        break;
                    }
                }
                break;
            }
        }
    }

    void Chip::loadProgram(const char *file) {
        std::ifstream program(file, std::ios::binary);
        if (!program.is_open()) {
            std::cout << "Failed to open program: " << file << std::endl;
            return;
        }
        std::copy(
            std::istreambuf_iterator<char>(program),
            std::istreambuf_iterator<char>(), std::begin(memory) + 0x200);
    }

    void Chip::decrementTimers()
    {
        if (soundTimer > 0) {
            soundTimer--;
        }

        if (delayTimer > 0) {
            delayTimer--;
        }
    }
} // namespace Chip8