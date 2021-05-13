include_directories(${CMAKE_SOURCE_DIR}/Libraries/asio)

set(CLIENT_HEADER_FILES
    Client.h
    BoostClient.h)
    
set(CLIENT_SOURCE_FILES
    Client.cpp
    BoostClient.cpp)

add_library(Client SHARED ${CLIENT_HEADER_FILES} ${CLIENT_SOURCE_FILES})

target_link_libraries(Client wsock32 ws2_32)