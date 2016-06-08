QT += core
QT -= gui

TARGET = createobjectfromname
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

CONFIG += c++11

HEADERS += createobjectfromname.hpp

DISTFILES += \
    readme.md
