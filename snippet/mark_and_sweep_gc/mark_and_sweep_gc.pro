QT += core
QT -= gui

TARGET = mark_and_sweep_gc
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    src/gc/gc.cpp \
    src/value.cpp \
    src/object.cpp \
    src/array.cpp \
    src/callable.cpp

CONFIG += c++11

HEADERS += \
    src/gc/gc.h \
    src/value.h \
    src/declara_forword.hpp \
    src/object.h \
    src/array.h \
    src/callable.h

DISTFILES += \
    src/gc/README

