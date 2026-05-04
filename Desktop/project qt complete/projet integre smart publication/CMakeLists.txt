cmake_minimum_required(VERSION 3.16)

project(ResearchManagementSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable verbose output for debugging
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set build type to Debug for testing
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Add compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ==================== Qt ====================
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql PrintSupport Charts Network SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql PrintSupport Charts Network SerialPort)

# ==================== OpenCV 4.5.5 MinGW ====================
# IMPORTANT: Modifier ce chemin selon votre installation locale
set(OpenCV_DIR "C:/Users/User/Desktop/OpenCV-MinGW-Build-OpenCV-4.5.5-x64")
find_package(OpenCV REQUIRED)

# Print Qt version for verification
message(STATUS "Using Qt version: ${QT_VERSION}")
message(STATUS "Qt Widgets found: ${Qt${QT_VERSION_MAJOR}Widgets_FOUND}")
message(STATUS "Qt Sql found: ${Qt${QT_VERSION_MAJOR}Sql_FOUND}")
message(STATUS "Qt Charts found: ${Qt${QT_VERSION_MAJOR}Charts_FOUND}")
message(STATUS "Qt PrintSupport found: ${Qt${QT_VERSION_MAJOR}PrintSupport_FOUND}")
message(STATUS "Qt Network found: ${Qt${QT_VERSION_MAJOR}Network_FOUND}")
message(STATUS "Qt SerialPort found: ${Qt${QT_VERSION_MAJOR}SerialPort_FOUND}")
# Source files
set(PROJECT_SOURCES
        # Main application
        main.cpp
        
        # Core classes
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        
        # Database connection
        connection.cpp
        connection.h
        
        # Authentication
        login.cpp
        login.h
        login.ui
        
        # User management (UPDATED)
        users.cpp
        users.h
        users.ui
        
        # Publications
        publicationspage.cpp
        publicationspage.h
        publicationspage.ui
        
        article.cpp
        article.h
        
        # Submissions
        submission.cpp
        submission.h
        submission.ui
        
        # Conferences
        conference.cpp
        conference.h
        conference.ui
        
        # Laboratories
        laboratoire.cpp
        laboratoire.h
        laboratoire.ui
        
        # Evaluations
        evaluation.cpp
        evaluation.h
        evaluation.ui
        
        # Arduino 1 - Décisions Soumissions (COM4)
        arduinocontroller.cpp
        arduinocontroller.h
        arduinomanager.cpp
        arduinomanager.h
        decisiondialog.cpp
        decisiondialog.h
        arduinopage.cpp
        arduinopage.h
        arduinopage.ui
        
        # Arduino 2 - Moniteur Température (COM5)
        arduinomonitor.cpp
        arduinomonitor.h
        arduinomonitor.ui
        
        # Arduino 3 - Identification Faciale (OpenCV + Webcam)
        faceauthdialog.cpp
        faceauthdialog.h
        facecapturedialog.cpp
        facecapturedialog.h
        arduino.cpp
        arduino.h
        
        # Resources
        SpyBot.qrc
)

# Print source files for verification
message(STATUS "Project sources configured:")
foreach(src ${PROJECT_SOURCES})
    message(STATUS "  - ${src}")
endforeach()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ResearchManagementSystem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
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

message(STATUS "Executable target 'ResearchManagementSystem' created")

target_link_libraries(ResearchManagementSystem PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets 
    Qt${QT_VERSION_MAJOR}::Sql 
    Qt${QT_VERSION_MAJOR}::PrintSupport 
    Qt${QT_VERSION_MAJOR}::Charts 
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::SerialPort
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
            "C:/Users/User/Desktop/OpenCV-MinGW-Build-OpenCV-4.5.5-x64/x64/mingw/bin"
            $<TARGET_FILE_DIR:ResearchManagementSystem>
    )
endif()

message(STATUS "Linked libraries:")
message(STATUS "  - Qt${QT_VERSION_MAJOR}::Widgets")
message(STATUS "  - Qt${QT_VERSION_MAJOR}::Sql")
message(STATUS "  - Qt${QT_VERSION_MAJOR}::PrintSupport")
message(STATUS "  - Qt${QT_VERSION_MAJOR}::Charts (for statistics)")
message(STATUS "  - Qt${QT_VERSION_MAJOR}::Network")
message(STATUS "  - Qt${QT_VERSION_MAJOR}::SerialPort (for Arduino)")

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
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

# Print build configuration summary
message(STATUS "")
message(STATUS "========================================")
message(STATUS "Build Configuration Summary")
message(STATUS "========================================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Qt Version: ${QT_VERSION}")
message(STATUS "")
message(STATUS "Features:")
message(STATUS "  ✓ User Management (CRUD + Search + Sort)")
message(STATUS "  ✓ Database Integration (Oracle)")
message(STATUS "  ✓ PDF Export")
message(STATUS "  ✓ Statistics with Charts")
message(STATUS "  ✓ Authentication System")
message(STATUS "  ✓ Arduino 1 - Décisions Soumissions (COM4)")
message(STATUS "  ✓ Arduino 2 - Moniteur Température (COM5)")
message(STATUS "  ✓ Arduino 3 - Identification Faciale (OpenCV + Webcam)")
message(STATUS "  ✓ OpenCV Integration (Face Recognition)")
message(STATUS "========================================")
message(STATUS "")
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libs: ${OpenCV_LIBS}")
