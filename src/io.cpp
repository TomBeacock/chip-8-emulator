#include "io.h"

#include <GLFW/glfw3.h>

namespace Chip8
{
    IO::IO()
    {
        keyMap[0x0] = glfwGetKeyScancode(GLFW_KEY_X);
        keyMap[0x1] = glfwGetKeyScancode(GLFW_KEY_1);
        keyMap[0x2] = glfwGetKeyScancode(GLFW_KEY_2);
        keyMap[0x3] = glfwGetKeyScancode(GLFW_KEY_3);
        keyMap[0x4] = glfwGetKeyScancode(GLFW_KEY_Q);
        keyMap[0x5] = glfwGetKeyScancode(GLFW_KEY_W);
        keyMap[0x6] = glfwGetKeyScancode(GLFW_KEY_E);
        keyMap[0x7] = glfwGetKeyScancode(GLFW_KEY_A);
        keyMap[0x8] = glfwGetKeyScancode(GLFW_KEY_S);
        keyMap[0x9] = glfwGetKeyScancode(GLFW_KEY_D);
        keyMap[0xA] = glfwGetKeyScancode(GLFW_KEY_Z);
        keyMap[0xB] = glfwGetKeyScancode(GLFW_KEY_C);
        keyMap[0xC] = glfwGetKeyScancode(GLFW_KEY_4);
        keyMap[0xD] = glfwGetKeyScancode(GLFW_KEY_R);
        keyMap[0xE] = glfwGetKeyScancode(GLFW_KEY_F);
        keyMap[0xF] = glfwGetKeyScancode(GLFW_KEY_V);
    }

    bool IO::isKeyDown(uint8_t n)
    {
        return keyPressedStates[n];
    }

    void IO::keyPressed(int scancode)
    {
        for (int i = 0; i < 16; i++) {
            if (keyMap[i] == scancode) {
                keyPressedStates[i] = true;
                break;
            }
        }
    }

    void IO::keyReleased(int scancode)
    {
        for (int i = 0; i < 16; i++) {
            if (keyMap[i] == scancode) {
                keyPressedStates[i] = false;
                lastReleased = i;
                break;
            }
        }
    }

    std::optional<uint8_t> IO::getReleasedKey()
    {
        if (lastReleased <= 0xF) {
            return lastReleased;
        }
        return {};
    }

    void IO::reset()
    {
        lastReleased = 255;
    }
} // namespace Chip8
