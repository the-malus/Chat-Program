set(SERVER_HEADER_FILES
    Server.h)
    
set(SERVER_SOURCE_FILES
    Server.cpp)

add_library(Server SHARED ${SERVER_HEADER_FILES} ${SERVER_SOURCE_FILES})

target_link_libraries(Server wsock32 ws2_32)