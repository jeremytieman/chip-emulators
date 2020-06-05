cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(chip-emulators)

add_subdirectory(src)

option(CXXOPTS_BUILD_CLI "Set to ON to build the CLI" ON)

if(CXXOPTS_BUILD_CLI)
  add_subdirectory(emulator)
endif()
