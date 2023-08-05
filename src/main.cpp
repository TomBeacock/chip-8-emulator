#include "chip.h"
#include "display.h"
#include "io.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <fstream>

using namespace Chip8;

constexpr int width = 64;
constexpr int height = 32;
constexpr int scale = 8;

int main(int argc, char **argv)
{
    glfwInit();

    GLFWwindow *window = glfwCreateWindow(
        width * scale, height * scale, "Chip-8 Emulator", nullptr, nullptr);
    glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
    glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    Display display(width, height);
    IO io;
    Chip chip(&display, &io);
    chip.loadProgram(argv[1]);

    glfwSetWindowUserPointer(window, &io);
    glfwSetKeyCallback(
        window,
        [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            IO *io = (IO *)glfwGetWindowUserPointer(window);
            switch (action) {
                case GLFW_PRESS:
                    io->keyPressed(scancode);
                    break;
                case GLFW_RELEASE:
                    io->keyReleased(scancode);
            }
    });

    double lastRefreshTime = 0.0;
    double lastInstructionTime = 0.0;
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();

        // Limit to 60hz
        if (currentTime - lastRefreshTime >= 1.0 / 60.0) {
            display.refresh();
            chip.decrementTimers();
            glfwSwapBuffers(window);
            lastRefreshTime = currentTime;
        }

        // Limit to 500hz
        if (currentTime - lastInstructionTime >= 1.0 / 500.0) {
            chip.runNext();
            io.reset();
            glfwPollEvents();
            lastInstructionTime = currentTime;
        }
    }

    glfwDestroyWindow(window);
}