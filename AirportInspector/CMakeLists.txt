cmake_minimum_required(VERSION 3.5)

project(AirportInspector VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets Sql Concurrent Charts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        secondaryform.h
        secondaryform.cpp
        secondaryform.ui
        db.h
        db.cpp
        stopwatch.h
        stopwatch.cpp
)

qt_add_executable(AirportInspector
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/PG_Libs)
target_link_libraries(AirportInspector PRIVATE Qt6::Widgets Qt6::Sql
                                                Qt6::Concurrent
                                                Qt6::Charts
                                                libcrypto-3-x64
                                                libiconv-2
                                                libintl-9
                                                libpq
                                                libssl-3-x64)

qt_finalize_executable(AirportInspector)

