QT += core
QT -= gui

TARGET = objectloop
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

CONFIG += c++11

HEADERS += \
    objectpool.hpp
