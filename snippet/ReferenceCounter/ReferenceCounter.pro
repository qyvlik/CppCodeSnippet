QT += core
QT -= gui

TARGET = ReferenceCounter
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

CONFIG += c++11

HEADERS += \
    src/referecnecountpointer.hpp \
    src/alias.hpp \
    test.hpp \
    test1.hpp
