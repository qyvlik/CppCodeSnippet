QT += core
QT -= gui

TARGET = throwable
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    src/calleetracker.hpp \
    src/threadstaticsingleton.hpp \
    src/throwable.hpp

CONFIG += c++11
