QT       += core gui widgets sql

CONFIG   += c++17
CONFIG   -= app_bundle

TEMPLATE = app
TARGET = project_name
INCLUDEPATH += .

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    second.cpp \
    third.cpp

HEADERS += \
    mainwindow.h \
    second.h \
    third.h

FORMS += \
    mainwindow.ui \
    second.ui \
    third.ui

QTPLUGIN += qsqlpsql
