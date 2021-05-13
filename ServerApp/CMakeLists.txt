include_directories(${CMAKE_SOURCE_DIR}/Server)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

add_executable(ServerApp ServerApp.cpp)

target_link_libraries(ServerApp Server)