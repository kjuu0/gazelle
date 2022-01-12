cmake_minimum_required(VERSION 3.10)

project(gazelle_client VERSION 1.0 LANGUAGES CXX)

find_package(Threads REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

add_executable(gazelle_client client.cpp)
target_link_libraries(gazelle_client rpc ${CMAKE_THREAD_LIBS_INIT})