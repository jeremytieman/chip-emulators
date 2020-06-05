cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(chip-emulators)

add_subdirectory(vendor)
add_subdirectory(src)

option(EMULATOR_BUILD_CLI "Set to ON to build the CLI" ON)

if(EMULATOR_BUILD_CLI)
  add_subdirectory(cli)
endif()
