#include "chip.h"
#include "display.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

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

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        display.refresh();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
}