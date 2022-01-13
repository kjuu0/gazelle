cmake_minimum_required(VERSION 3.10)

project(pa_test VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

find_package(portaudio)

add_executable(pa_devs pa_devs.c)
target_link_libraries(pa_devs portaudio_static)

add_executable(paex_record paex_record.c)
target_link_libraries(paex_record portaudio_static)

add_executable(beep beep.c)
target_link_libraries(beep portaudio_static)

add_executable(echo echo.c)
target_link_libraries(echo portaudio_static)