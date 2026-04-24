cmake_minimum_required(VERSION 3.16)

project(ResearchManagementSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==================== Qt ====================
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql PrintSupport Charts Network SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql PrintSupport Charts Network SerialPort)

# ==================== OpenCV 4.5.5 MinGW ====================
set(OpenCV_DIR "C:/Users/Azzabi Alaedinne/Desktop/Projet QT/S9/OpenCV-MinGW-Build-OpenCV-4.5.5-x64")
find_package(OpenCV REQUIRED)

# ==================== Project Sources ====================
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        publicationspage.cpp
        submission.cpp
        conference.cpp
        laboratoire.cpp
        connection.cpp
        article.cpp
        users.cpp
        login.cpp
        facecapturedialog.cpp
        faceauthdialog.cpp
        arduino.cpp           # ← NEW
        faceauthdialog.h
        facecapturedialog.h
        mainwindow.h
        publicationspage.h
        submission.h
        conference.h
        laboratoire.h
        connection.h
        article.h
        users.h
        login.h
        arduino.h             # ← NEW
        mainwindow.ui
        publicationspage.ui
        submission.ui
        conference.ui
        laboratoire.ui
        users.ui
        login.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ResearchManagementSystem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        SpyBot.qrc
    )
else()
    if(ANDROID)
        add_library(ResearchManagementSystem SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(ResearchManagementSystem
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# ==================== Linking ====================
target_link_libraries(ResearchManagementSystem PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::Charts
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::SerialPort   # ← NEW
    ${OpenCV_LIBS}
)

# ==================== Include Dirs ====================
target_include_directories(ResearchManagementSystem PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)

# ==================== Windows: Copy OpenCV DLLs after build ====================
if(WIN32)
    add_custom_command(TARGET ResearchManagementSystem POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "C:/Users/Azzabi Alaedinne/Desktop/Projet QT/S9/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/x64/mingw/bin"
            $<TARGET_FILE_DIR:ResearchManagementSystem>
    )
endif()

# ==================== Target Properties ====================
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.research.ResearchManagementSystem)
endif()
set_target_properties(ResearchManagementSystem PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ResearchManagementSystem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ResearchManagementSystem)
endif()

# ==================== Debug Info ====================
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libs: ${OpenCV_LIBS}")
