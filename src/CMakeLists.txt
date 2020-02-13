set(SFML_DIR ${CMAKE_SOURCE_DIR}/external/SFML-2.5.1)

add_library(ChipEmulators chip8.cpp)
target_include_directories(ChipEmulators PRIVATE ../inc)
set_target_properties(ChipEmulators PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/)

list(APPEND OptimizedSFMLLibs "optimized" "sfml-graphics.lib" "optimized" "sfml-window.lib" "optimized" "sfml-system.lib")
list(APPEND DebugSFMLLibs "debug" "sfml-graphics-d.lib" "debug" "sfml-window-d.lib" "debug" "sfml-system-d.lib")

add_executable(emulator emulator.cpp)
target_include_directories(emulator PRIVATE ../inc ${PROJECT_BINARY_DIR}/external/cxxopts-src/include ${SFML_DIR}/include)
target_link_directories(emulator PRIVATE ${SFML_DIR}/lib)
target_link_libraries(emulator ChipEmulators ${OptimizedSFMLLibs} ${DebugSFMLLibs})
set_target_properties(emulator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/)

list(APPEND SFMLDebugDlls ${SFML_DIR}/bin/sfml-audio-d-2.dll ${SFML_DIR}/bin/sfml-graphics-d-2.dll ${SFML_DIR}/bin/sfml-network-d-2.dll ${SFML_DIR}/bin/sfml-system-d-2.dll ${SFML_DIR}/bin/sfml-window-d-2.dll)
list(APPEND SFMLReleaseDlls ${SFML_DIR}/bin/sfml-audio-2.dll ${SFML_DIR}/bin/sfml-graphics-2.dll ${SFML_DIR}/bin/sfml-network-2.dll ${SFML_DIR}/bin/sfml-system-2.dll ${SFML_DIR}/bin/sfml-window-2.dll)
list(APPEND Dlls ${SFML_DIR}/bin/openal32.dll)
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

foreach(file ${Dlls})
add_custom_command(
  TARGET emulator
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${file} $<TARGET_FILE_DIR:emulator>
)
endforeach()