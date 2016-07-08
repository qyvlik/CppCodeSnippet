QT += core
QT -= gui

TARGET = colorful_cout
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

HEADERS += \
    src/colorconsole.hpp

CONFIG += c++11

