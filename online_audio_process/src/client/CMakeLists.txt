cmake_minimum_required(VERSION 3.10)

project(Addra VERSION 1.1 LANGUAGES CXX)

# Executable will be in ./bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories( ${CMAKE_SOURCE_DIR}/../common)
include_directories(/usr/local/include)
link_directories(/usr/local/lib)
link_directories((/usr/lib/x86_64-linux-gnu))
set(DYNAMIC_LIBRARIES libportaudio.so liblpcnet.so libpthread.so)

find_package(SEAL 3.5 REQUIRED)

add_executable(client client.cpp )
target_link_libraries(client SEAL::seal)
target_link_libraries(client SEAL::seal rpc)
target_link_libraries(client SEAL::seal crypto)
target_link_libraries(client ${DYNAMIC_LIBRARIES})