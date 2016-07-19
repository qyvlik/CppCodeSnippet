QT += core
QT -= gui

TARGET = json_like_value
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    src/gc/gc.cpp \
    src/domain/array.cpp \
    src/domain/callable.cpp \
    src/domain/object.cpp \
    src/domain/value.cpp \
    src/gc/trigger.cpp

HEADERS += \
    src/declara_forword.hpp \
    src/gc/gc.h \
    src/domain/array.h \
    src/domain/callable.h \
    src/domain/object.h \
    src/domain/value.h \
    src/gc/trigger.h

DISTFILES += \
    src/gc/README

