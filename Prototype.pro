QT       += core gui widgets sql

CONFIG   += c++17
CONFIG   -= app_bundle

TEMPLATE = app
TARGET = project_name
INCLUDEPATH += .

SOURCES += \
    fourth.cpp \
    main.cpp \
    mainwindow.cpp \
    second.cpp \
    third.cpp

HEADERS += \
    fourth.h \
    mainwindow.h \
    second.h \
    third.h

FORMS += \
    fourth.ui \
    mainwindow.ui \
    second.ui \
    third.ui

QTPLUGIN += qsqlpsql

RESOURCES += \
    resources.qrc
