cmake_minimum_required(VERSION 3.12)

project(GLAD)

add_library(glad "include/glad/gl.h" "include/KHR/khrplatform.h" "src/gl.c")

target_include_directories(glad PRIVATE "${PROJECT_SOURCE_DIR}/include")