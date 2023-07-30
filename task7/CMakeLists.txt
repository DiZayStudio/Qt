cmake_minimum_required(VERSION 3.5)

project(PreGraphic VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets
                                     Concurrent
                                     Charts  )


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)


add_executable(PreGraphic
    ${PROJECT_SOURCES}
)


target_link_libraries(PreGraphic Qt6::Widgets
                                 Qt6::Concurrent
                                 Qt6::Charts  )

