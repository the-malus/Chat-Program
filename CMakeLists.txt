cmake_minimum_required(VERSION 3.10)

project(ChatSystem)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(BUILD_SHARED_LIBS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(Logger)
add_subdirectory(Server)
add_subdirectory(ServerApp)
add_subdirectory(Client)
add_subdirectory(ClientApp)

add_dependencies(ServerApp Server)
add_dependencies(ClientApp Client)
add_dependencies(Server Logger)
add_dependencies(Client Logger)