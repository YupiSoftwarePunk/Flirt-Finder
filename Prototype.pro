QT       += core gui widgets sql

CONFIG   += c++17
CONFIG   -= app_bundle

TEMPLATE = app
TARGET = project_name
INCLUDEPATH += .

SOURCES += \
    .\src\fourth.cpp \
    .\src\fifth.cpp \
    main.cpp \
    .\src\mainwindow.cpp \
    .\src\second.cpp \
    .\src\third.cpp

HEADERS += \
    .\include\fourth.h \
    .\include\mainwindow.h \
    .\include\second.h \
    .\include\third.h \
    .\include\fifth.h

FORMS += \
    .\forms\fourth.ui \
    .\forms\mainwindow.ui \
    .\forms\second.ui \
    .\forms\third.ui \
    .\forms\fifth.ui

QTPLUGIN += qsqlpsql

RESOURCES += \
    resources.qrc
