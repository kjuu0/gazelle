cmake_minimum_required(VERSION 3.10)

project(gazelle_server VERSION 1.0 LANGUAGES C CXX)

find_package(Threads REQUIRED)
find_package(SEAL 3.7 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/../common)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(gazelle_server server.cpp)
add_executable(gazelle_worker worker.cpp)

target_link_libraries(gazelle_server SEAL::seal)
target_link_libraries(gazelle_server rpc ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(gazelle_worker SEAL::seal)
target_link_libraries(gazelle_worker rpc ${CMAKE_THREAD_LIBS_INIT})

