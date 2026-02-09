cmake_minimum_required(VERSION 3.16)

project(GestionSoumission VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Source files
set(PROJECT_SOURCES
        main.cpp
        submission.cpp
)

# Header files
set(PROJECT_HEADERS
        submission.h
)

# Form files
set(PROJECT_FORMS
        submission.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GestionSoumission
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_FORMS}
    )
else()
    if(ANDROID)
        add_library(GestionSoumission SHARED
            ${PROJECT_SOURCES}
            ${PROJECT_HEADERS}
            ${PROJECT_FORMS}
        )
    else()
        add_executable(GestionSoumission
            ${PROJECT_SOURCES}
            ${PROJECT_HEADERS}
            ${PROJECT_FORMS}
        )
    endif()
endif()

target_link_libraries(GestionSoumission PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Organize files into folders in IDE
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Forms" FILES ${PROJECT_FORMS})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GestionSoumission)
endif()
