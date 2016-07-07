QT += core
QT -= gui

TARGET = stack_base_virtual_machine
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    src/gc/gc.cpp \
    src/domain/array.cpp \
    src/domain/callable.cpp \
    src/domain/object.cpp \
    src/domain/value.cpp \
    src/engine/virtual_machine/virtualmachine.cpp \
    src/engine/context/virtualmachinecontext.cpp

CONFIG += c++11

DISTFILES += \
    README.md \
    src/gc/README

HEADERS += \
    src/gc/gc.h \
    src/domain/array.h \
    src/domain/callable.h \
    src/domain/object.h \
    src/domain/value.h \
    src/engine/virtual_machine/virtualmachine.h \
    src/engine/context/virtualmachinecontext.h \
    src/declara_forword.hpp
