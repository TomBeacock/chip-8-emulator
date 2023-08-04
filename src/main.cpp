#include "chip.h"
#include "display.h"

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
    // TODO toggle decoration
    //glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);

    Display display(width, height);
    Chip chip(&display);
    chip.loadProgram(argv[1]);

    double lastRefresh = 0.0;
    double lastInstruction = 0.0;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        double currentTime = glfwGetTime();

        // Limit to 700hz
        if (currentTime - lastInstruction >= 1.0 / 700.0) {
            chip.runNext();
            lastInstruction = currentTime;
        }

        // Limit to 60hz
        if (currentTime - lastRefresh >= 1.0 / 60.0) {
            display.refresh();
            chip.decrementTimers();
            glfwSwapBuffers(window);
            lastRefresh = currentTime;
        }
    }

    glfwDestroyWindow(window);
}