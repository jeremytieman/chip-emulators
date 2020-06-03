add_subdirectory(vendor)

project(emulator LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
add_executable(emulator emulator.cpp)
target_link_libraries(emulator cxxopts ChipEmulators sfml-system sfml-window sfml-graphics)
set_target_properties(emulator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../bin/)

set(SFML_RELEASE_DIR ${SFML_BINARY_DIR}/lib/Release)
set(SFML_DEBUG_DIR ${SFML_BINARY_DIR}/lib/Debug)

list(APPEND SFMLDebugDlls ${SFML_DEBUG_DIR}/sfml-graphics-d-2.dll ${SFML_DEBUG_DIR}/bin/sfml-system-d-2.dll ${SFML_DEBUG_DIR}/bin/sfml-window-d-2.dll)
list(APPEND SFMLReleaseDlls ${SFML_RELEASE_DIR}/sfml-graphics-2.dll ${SFML_RELEASE_DIR}/sfml-system-2.dll ${SFML_RELEASE_DIR}/sfml-window-2.dll)

set(cmd_arg_1 -E)
set(cmd_arg_2 copy_if_different)

foreach(file ${SFMLDebugDlls})
add_custom_command(
  TARGET emulator
  POST_BUILD
  COMMAND $<$<CONFIG:Debug>:${CMAKE_COMMAND}> $<$<CONFIG:Debug>:${cmd_arg_1}> $<$<CONFIG:Debug>:${cmd_arg_2}> $<$<CONFIG:Debug>:${file}> $<$<CONFIG:Debug>:$<TARGET_FILE_DIR:emulator>>
)
endforeach()

foreach(file ${SFMLReleaseDlls})
add_custom_command(
  TARGET emulator
  POST_BUILD
  COMMAND $<$<NOT:$<CONFIG:Debug>>:${CMAKE_COMMAND}> $<$<NOT:$<CONFIG:Debug>>:${cmd_arg_1}> $<$<NOT:$<CONFIG:Debug>>:${cmd_arg_2}> $<$<NOT:$<CONFIG:Debug>>:${file}> $<$<NOT:$<CONFIG:Debug>>:$<TARGET_FILE_DIR:emulator>>
)
endforeach()
