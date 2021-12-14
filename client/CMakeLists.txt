cmake_minimum_required(VERSION 3.10)

project(gazelle_client VERSION 1.0 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

add_executable(gazelle client.cpp)