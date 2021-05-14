include_directories(
    ${CMAKE_SOURCE_DIR}/Server
    ${CMAKE_SOURCE_DIR}/Logger)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

add_executable(ServerApp ServerApp.cpp)

target_link_libraries(ServerApp Server)