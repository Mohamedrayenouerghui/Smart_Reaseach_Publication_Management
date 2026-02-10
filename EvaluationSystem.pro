QT       += core gui sql charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/evaluationlistwindow.cpp \
    src/evaluationformdialog.cpp \
    src/reviewerwindow.cpp \
    src/statisticswindow.cpp \
    src/database.cpp \
    src/evaluation.cpp \
    src/reviewer.cpp

HEADERS += \
    headers/mainwindow.h \
    headers/evaluationlistwindow.h \
    headers/evaluationformdialog.h \
    headers/reviewerwindow.h \
    headers/statisticswindow.h \
    headers/database.h \
    headers/evaluation.h \
    headers/reviewer.h

FORMS += \
    ui/mainwindow.ui \
    ui/evaluationlistwindow.ui \
    ui/evaluationformdialog.ui \
    ui/reviewerwindow.ui \
    ui/statisticswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc
