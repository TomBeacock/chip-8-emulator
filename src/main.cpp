#include "chip.h"
#include "display.h"

#include <iostream>

using namespace Chip8;

int main(int argc, char **argv)
{
    Display display(64, 32);
    Chip chip(&display);
    chip.runProgram();
    std::cout << "Finished" << std::endl;
}