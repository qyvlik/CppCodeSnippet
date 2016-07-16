QT += core
QT -= gui

TARGET = on_happend_and_handle
CONFIG += console
CONFIG -= app_bundle

CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    src/trigger.cpp

HEADERS += \
    src/trigger.h

