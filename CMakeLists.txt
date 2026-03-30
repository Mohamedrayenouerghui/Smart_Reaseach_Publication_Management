cmake_minimum_required(VERSION 3.16)

project(ResearchManagementSystem VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ====================== Find Qt Packages ======================
# We need Widgets, Sql, PrintSupport (for publicationspage), and Charts (for statisticspage)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql PrintSupport Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql PrintSupport Charts)

# ====================== Source Files ======================
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    login.cpp
    connection.cpp
    users.cpp
    statisticspage.cpp
    publicationspage.cpp
    submission.cpp
    conference.cpp
    laboratoire.cpp
    evaluation.cpp
    evaluationpage.cpp
    evaluationformdialog.cpp
    database.cpp
    reviewer.cpp
    article.cpp

    mainwindow.h
    login.h
    connection.h
    users.h
    statisticspage.h
    publicationspage.h
    submission.h
    conference.h
    laboratoire.h
    evaluation.h
    evaluationpage.h
    evaluationformdialog.h
    database.h
    reviewer.h
    article.h

    mainwindow.ui
    login.ui
    users.ui
    statisticspage.ui
    publicationspage.ui
    submission.ui
    conference.ui
    laboratoire.ui
    evaluationpage.ui
    evaluationformdialog.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ResearchManagementSystem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        SpyBot.qrc
    )
else()
    if(ANDROID)
        add_library(ResearchManagementSystem SHARED ${PROJECT_SOURCES})
    else()
        add_executable(ResearchManagementSystem ${PROJECT_SOURCES})
    endif()
endif()

# ====================== Link Libraries ======================
target_link_libraries(ResearchManagementSystem PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::Charts          # ← This fixes the QChart / QPieSeries errors
)

# Bundle settings
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
