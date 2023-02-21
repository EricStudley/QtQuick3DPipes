cmake_minimum_required(VERSION 3.16)

project(QtQuick3DPipes VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)

qt_add_executable(QtQuick3DPipes
    qml.qrc
    src/main.cpp
    src/PipeEnums.h
    src/PipeObject.h
    src/PipeObject.cpp
    src/PipeModel.h
    src/PipeModel.cpp
)

set_target_properties(QtQuick3DPipes PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(QtQuick3DPipes
    PRIVATE Qt6::Quick)

install(TARGETS QtQuick3DPipes
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
