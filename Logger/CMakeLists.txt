set(LOGGER_HEADER_FILES
    Logger.h)
    
set(LOGGER_SOURCE_FILES
    Logger.cpp)

add_library(Logger SHARED ${LOGGER_HEADER_FILES} ${LOGGER_SOURCE_FILES})