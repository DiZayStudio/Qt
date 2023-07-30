cmake_minimum_required(VERSION 3.5)

project(Concurrent VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets
                                     Core
                                     Concurrent)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ExampleRace.cpp
        ExampleRace.h
)


qt_add_executable(Concurrent
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(Concurrent PRIVATE Qt6::Widgets
                                         Qt6::Core
                                         Qt6::Concurrent)

qt_finalize_executable(Concurrent)
