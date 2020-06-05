project(ChipEmulators VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

add_library(ChipEmulators chip8.cpp)
target_include_directories(ChipEmulators PUBLIC ../inc)
set_target_properties(ChipEmulators PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../lib/)
