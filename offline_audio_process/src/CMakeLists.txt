INCLUDE_DIRECTORIES(/usr/local/include)
LINK_DIRECTORIES(/usr/local/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
# SET(DYNAMIC_LIBRARIES libportaudio.dylib liblpcnet.dylib)
SET(DYNAMIC_LIBRARIES libportaudio.so liblpcnet.so libpthread.so)

ADD_EXECUTABLE(paex_record paex_record.c)
TARGET_LINK_LIBRARIES(paex_record ${DYNAMIC_LIBRARIES})

ADD_EXECUTABLE(playback_record playback_record.c)
TARGET_LINK_LIBRARIES(playback_record ${DYNAMIC_LIBRARIES})

ADD_EXECUTABLE(Dialling Dialling.c)
TARGET_LINK_LIBRARIES(Dialling ${DYNAMIC_LIBRARIES})

ADD_EXECUTABLE(Dialling2 Dialling2.c)
TARGET_LINK_LIBRARIES(Dialling2 ${DYNAMIC_LIBRARIES})

ADD_EXECUTABLE(Dialling3 Dialling3.c)
TARGET_LINK_LIBRARIES(Dialling3 ${DYNAMIC_LIBRARIES})