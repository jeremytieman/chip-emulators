project(cli VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

option(CXXOPTS_BUILD_EXAMPLES OFF)
option(CXXOPTS_BUILD_TESTS OFF)

set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
option(BUILD_SHARED_LIBS OFF)

include(FetchContent)
FetchContent_MakeAvailable(cxxopts SFML)

add_executable(cli cli.cpp)
target_link_libraries(cli PRIVATE cxxopts ChipEmulators sfml-system sfml-window sfml-graphics)
set_target_properties(cli PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../bin/)
