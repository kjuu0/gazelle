INCLUDE_DIRECTORIES(/usr/local/include)
LINK_DIRECTORIES(/usr/local/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
SET(DYNAMIC_LIBRARIES libportaudio.dylib liblpcnet.dylib)


# ADD_EXECUTABLE(pa_devs pa_devs.c)
# TARGET_LINK_LIBRARIES(pa_devs ${DYNAMIC_LIBRARIES})

# ADD_EXECUTABLE(paex_record paex_record.c)
# TARGET_LINK_LIBRARIES(paex_record ${DYNAMIC_LIBRARIES})

# ADD_EXECUTABLE(beep beep.c)
# TARGET_LINK_LIBRARIES(beep ${DYNAMIC_LIBRARIES})

# ADD_EXECUTABLE(echo echo.c)
# TARGET_LINK_LIBRARIES(echo ${DYNAMIC_LIBRARIES})

# ADD_EXECUTABLE(playback_record playback_record.c)
# TARGET_LINK_LIBRARIES(playback_record ${DYNAMIC_LIBRARIES})

ADD_EXECUTABLE(paex_read_write_wire paex_read_write_wire.c)
TARGET_LINK_LIBRARIES(paex_read_write_wire ${DYNAMIC_LIBRARIES})

# include_directories( ${CMAKE_SOURCE_DIR}/../common)
# change this to find global.h

# find_package(SEAL 3.5 REQUIRED)
# target_link_libraries(paex_read_write_wire SEAL::seal)
# target_link_libraries(paex_read_write_wire SEAL::seal rpc)
# target_link_libraries(paex_read_write_wire SEAL::seal crypto)