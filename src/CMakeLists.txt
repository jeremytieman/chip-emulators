include_directories(../inc)
include_directories(../external/boost_1_69_0)
include_directories(../external/SFML-2.5.1/include)

link_directories(../external/boost_1_69_0/lib)
link_directories(../external/SFML-2.5.1/lib)

add_library(ChipEmulators chip8.cpp)

list(APPEND OptimizedBoostLibs "libboost_program_options-vc141-mt-x64-1_69.lib")
list(APPEND DebugBoostLibs "libboost_program_options-vc141-mt-gd-x64-1_69.lib")

list(APPEND OptimizedSFMLLibs "sfml-graphics.lib" "sfml-window.lib" "sfml-system.lib")
list(APPEND DebugSFMLLibs "sfml-graphics-d.lib" "sfml-window-d.lib" "sfml-system-d.lib")

add_executable(emulator emulator.cpp)
target_link_libraries(emulator ChipEmulators optimized ${OptimizedBoostLibs} ${OptimizedSFMLLibs} debug ${DebugBoostLibs} ${DebugSFMLLibs})