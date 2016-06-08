QT += core
QT -= gui

TARGET = threadstaticsingleton
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

CONFIG += c++11

DISTFILES += \
    readme.md

HEADERS += \
    threadstaticsingleton.hpp
