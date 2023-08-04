#pragma once

#include <cstdint>
#include <vector>
#include <glad/gl.h>

struct GLFWwindow;

namespace Chip8
{
    class Display
    {
      public:
        Display(uint8_t width, uint8_t height);

        void refresh();
        void drawSprite(uint8_t *src, uint8_t n, uint8_t x, uint8_t y);
        void clear();

      private:
        uint8_t width;
        uint8_t height;
        GLuint display;
        std::vector<uint8_t> pixels;
        GLuint mesh;
        GLuint shader;
        bool dirty;
    };
} // namespace Chip8