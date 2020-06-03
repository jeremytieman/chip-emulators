include(FetchContent)

option(CXXOPTS_BUILD_EXAMPLES OFF)
option(CXXOPTS_BUILD_TESTS OFF)

FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jeremytieman/cxxopts.git
  GIT_TAG v2.2.0
)

if(MSVC)
  add_compile_definitions(_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/jeremytieman/SFML.git
  GIT_TAG 2.5.1
)

FetchContent_MakeAvailable(cxxopts SFML)
