INCLUDE_DIRECTORIES(/usr/local/include)
LINK_DIRECTORIES(/usr/local/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
SET(DYNAMIC_LIBRARIES libportaudio.dylib)

ADD_EXECUTABLE(paex_record paex_record.c)
TARGET_LINK_LIBRARIES(paex_record ${DYNAMIC_LIBRARIES})

ADD_EXECUTABLE(playback_record playback_record.c)
TARGET_LINK_LIBRARIES(playback_record ${DYNAMIC_LIBRARIES})