#include "display.h"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <glad/gl.h>
#include <iostream>

namespace Chip8
{
    GLuint createTexture(GLsizei width, GLsizei height);
    GLuint createShader();
    GLuint createMesh();

    Display::Display(uint8_t width, uint8_t height)
        : width(width),
          height(height),
          pixels(width * height, 0),
          dirty(false)
    {
        display = createTexture(width, height);
        glBindTexture(GL_TEXTURE_2D, display);

        shader = createShader();
        glUseProgram(shader);

        mesh = createMesh();
        glBindVertexArray(mesh);
    }

    void Display::refresh()
    {
        // Update display
        glTextureSubImage2D(
            display, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE,
            pixels.data());
        // Draw display
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void Display::drawSprite(uint8_t *src, uint8_t n, uint8_t x, uint8_t y)
    {
        // Wrap coord to screen
        x %= width;
        y %= height;

        // Iterate pixels
        for (size_t i = 0, dy = y; dy < std::min((uint8_t)(y + n), height);
             i++, dy++) {
            uint8_t byte = src[i];
            uint8_t mask = 0x80;
            for (size_t dx = x; dx < std::min((uint8_t)(x + 8), width);
                 dx++, mask >>= 1) {
                if ((byte & mask) > 0) {
                    // Flip pixel
                    pixels[dy * width + dx] = ~pixels[dy * width + dx];
                }
            }
        }
        dirty = true;
    }

    void Display::clear()
    {
        for (size_t i = 0; i < width * height; i++) {
            pixels[i] = 0;
        }
    }

    GLuint createTexture(GLsizei width, GLsizei height)
    {
        GLuint texture;
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureStorage2D(texture, 1, GL_R8, width, height);
        return texture;
    }

    GLuint createShader()
    {
        GLint success;
        GLchar infoLog[512];

        const GLchar *vertexSource = R"(
#version 450 core

layout (location = 0) in vec2 v_in_pos;
layout (location = 1) in vec2 v_in_uv;

out vec2 v_out_uv;

void main() {
    v_out_uv = v_in_uv;
    gl_Position = vec4(v_in_pos, 0.0, 1.0);
})";

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSource, nullptr);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            std::cerr << "Vertex Shader Compilation Fail: " << infoLog
                      << std::endl;
        }

        const GLchar *fragmentSource = R"(
#version 450 core

in vec2 v_out_uv;

out vec4 f_out_color;

uniform sampler2D u_display;

void main() {
    vec3 value = vec3(texture(u_display, v_out_uv).r);
    f_out_color = vec4(value, 1.0);
})";

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cerr << "Fragment Shader Compilation Fail: " << infoLog
                      << std::endl;
        }

        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

    GLuint createMesh()
    {
        // Create vertex buffer
        float vertices[6 * 4] = {-1.0f, 1.0f, 0.0f, 0.0f,  -1.0f, -1.0f,
                                 0.0f,  1.0f, 1.0f, -1.0f, 1.0f,  1.0f,
                                 -1.0f, 1.0f, 0.0f, 0.0f,  1.0f,  -1.0f,
                                 1.0f,  1.0f, 1.0f, 1.0f,  1.0f,  0.0f};

        GLuint vertexBuffer;
        glCreateBuffers(1, &vertexBuffer);
        glNamedBufferData(
            vertexBuffer, sizeof(vertices), &vertices, GL_STATIC_DRAW);

        // Create vertex array
        GLuint vertexArray;
        glCreateVertexArrays(1, &vertexArray);

        glVertexArrayAttribBinding(vertexArray, 0, 0);
        glVertexArrayAttribFormat(vertexArray, 0, 2, GL_FLOAT, GL_FALSE, 0);
        glEnableVertexArrayAttrib(vertexArray, 0);

        glVertexArrayAttribBinding(vertexArray, 1, 0);
        glVertexArrayAttribFormat(
            vertexArray, 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));
        glEnableVertexArrayAttrib(vertexArray, 1);

        // Bind vertex buffer
        glVertexArrayVertexBuffer(
            vertexArray, 0, vertexBuffer, 0, 4 * sizeof(float));

        return vertexArray;
    }

} // namespace Chip8