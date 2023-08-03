cmake_minimum_required(VERSION 3.12)

project(Chip8)

add_subdirectory("extern/glad")
add_subdirectory("extern/glfw")

add_executable(chip8
"src/main.cpp"
"src/chip.h" "src/chip.cpp"
"src/display.h" "src/display.cpp")

target_link_libraries(chip8 PRIVATE glfw glad)
target_include_directories(chip8 PRIVATE "extern/glad/include" "extern/glfw/include")