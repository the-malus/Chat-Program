include_directories(${CMAKE_SOURCE_DIR}/Logger)
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(CLIENT_HEADER_FILES
    Client.h)
    
set(CLIENT_SOURCE_FILES
    Client.cpp)

add_library(Client SHARED ${CLIENT_HEADER_FILES} ${CLIENT_SOURCE_FILES})

target_link_libraries(Client 
    wsock32 
    ws2_32 
    Logger)