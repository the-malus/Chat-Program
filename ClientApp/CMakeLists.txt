include_directories(
    ${CMAKE_SOURCE_DIR}/Client
    ${CMAKE_SOURCE_DIR}/Logger)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set(CLIENTAPP_SOURCES
    ClientApp.cpp
    ClientWindow.cpp
    ClientWindow.h
    ClientWindow.ui
    #ClientLogin.cpp
    #ClientLogin.h
    #ClientLogin.ui
)

add_executable(ClientApp ${CLIENTAPP_SOURCES})

target_link_libraries(ClientApp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Client)