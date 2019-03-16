include_directories(../inc)
include_directories(../external/boost_1_69_0)
include_directories(../external/SDL2-2.0.9/include)

link_directories(../external/boost_1_69_0/lib)
link_directories(../external/SDL2-2.0.9/lib)

add_library(ChipEmulators chip8.cpp)

add_executable(emulator emulator.cpp)
target_link_libraries(emulator ChipEmulators optimized libboost_program_options-vc141-mt-x64-1_69.lib debug libboost_program_options-vc141-mt-gd-x64-1_69.lib)