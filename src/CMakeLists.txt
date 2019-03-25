set(BOOST_DIR ../external/boost_1_69_0)
set(SFML_DIR ../external/SFML-2.5.1)

include_directories(../inc)
include_directories(${BOOST_DIR})
include_directories(${SFML_DIR}/include)

link_directories(${BOOST_DIR}/lib)
link_directories(${SFML_DIR}/lib)

add_library(ChipEmulators chip8.cpp)

list(APPEND OptimizedBoostLibs "optimized" "libboost_program_options-vc141-mt-x64-1_69.lib")
list(APPEND DebugBoostLibs "debug" "libboost_program_options-vc141-mt-gd-x64-1_69.lib")

list(APPEND OptimizedSFMLLibs "optimized" "sfml-graphics.lib" "optimized" "sfml-window.lib" "optimized" "sfml-system.lib")
list(APPEND DebugSFMLLibs "debug" "sfml-graphics-d.lib" "debug" "sfml-window-d.lib" "debug" "sfml-system-d.lib")

add_executable(emulator emulator.cpp)
target_link_libraries(emulator ChipEmulators ${OptimizedBoostLibs} ${OptimizedSFMLLibs} ${DebugBoostLibs} ${DebugSFMLLibs})

list(APPEND SFMLDebugDlls sfml-audio-d-2.dll sfml-graphics-d-2.dll sfml-network-d-2.dll sfml-system-d-2.dll sfml-window-d-2.dll)
list(APPEND SFMLReleaseDlls sfml-audio-2.dll sfml-graphics-2.dll sfml-network-2.dll sfml-system-2.dll sfml-window-2.dll)
list(APPEND Dlls openal32.dll)

message(STATUS ${CMAKE_BUILD_TYPE})

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  list(APPEND Dlls ${SFMLDebugDlls})
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
  list(APPEND Dlls ${SFMLReleaseDlls})
endif()

foreach(file ${Dlls})
configure_file(${SFML_DIR}/bin/${file} ${CMAKE_CURRENT_BINARY_DIR}/${file} COPYONLY)
endforeach()