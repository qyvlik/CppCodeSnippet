QT += core
QT -= gui

TARGET = calleetracker
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

CONFIG += c++11

HEADERS += \
    calleetracker.hpp \
    threadstaticsingleton.hpp

DISTFILES += \
    readme.md

