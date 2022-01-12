cmake_minimum_required(VERSION 3.10)

project(gazelle_server VERSION 1.0 LANGUAGES C CXX)

find_package(Threads REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

add_executable(gazelle_server server.cpp)
target_link_libraries(gazelle_server rpc ${CMAKE_THREAD_LIBS_INIT})