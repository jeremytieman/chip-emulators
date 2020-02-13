cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(chip-emulators LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external)
add_subdirectory(src)
