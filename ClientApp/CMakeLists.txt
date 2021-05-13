include_directories(${CMAKE_SOURCE_DIR}/Client
    ${CMAKE_SOURCE_DIR}/Libraries/asio)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

add_executable(ClientApp ClientApp.cpp)

target_link_libraries(ClientApp Client)