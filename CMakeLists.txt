cmake_minimum_required(VERSION 3.12)

project("chip-8")

add_executable(chip8
"src/main.cpp"
"src/chip.h" "src/chip.cpp"
"src/display.h" "src/display.cpp")