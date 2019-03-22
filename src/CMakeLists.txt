include_directories(../inc)
include_directories(../external/boost_1_69_0)
include_directories(../external/SFML-2.5.1/include)

link_directories(../external/boost_1_69_0/lib)
link_directories(../external/SFML-2.5.1/lib)

add_library(ChipEmulators chip8.cpp)

list(APPEND OptimizedBoostLibs "optimized" "libboost_program_options-vc141-mt-x64-1_69.lib")
list(APPEND DebugBoostLibs "debug" "libboost_program_options-vc141-mt-gd-x64-1_69.lib")

list(APPEND OptimizedSFMLLibs "optimized" "sfml-graphics.lib" "optimized" "sfml-window.lib" "optimized" "sfml-system.lib")
list(APPEND DebugSFMLLibs "debug" "sfml-graphics-d.lib" "debug" "sfml-window-d.lib" "debug" "sfml-system-d.lib")

add_executable(emulator emulator.cpp)
target_link_libraries(emulator ChipEmulators ${OptimizedBoostLibs} ${OptimizedSFMLLibs} ${DebugBoostLibs} ${DebugSFMLLibs})