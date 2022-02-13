INCLUDE_DIRECTORIES(/usr/local/include)
LINK_DIRECTORIES(/usr/local/lib)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

ADD_EXECUTABLE(pa_devs pa_devs.c)
TARGET_LINK_LIBRARIES(pa_devs portaudio_static)

ADD_EXECUTABLE(paex_record paex_record.c)
TARGET_LINK_LIBRARIES(paex_record portaudio_static)

ADD_EXECUTABLE(beep beep.c)
TARGET_LINK_LIBRARIES(beep portaudio_static)

ADD_EXECUTABLE(echo echo.c)
TARGET_LINK_LIBRARIES(echo portaudio_static)