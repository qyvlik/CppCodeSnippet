QT += core
QT -= gui

TARGET = my_back_trace
CONFIG += console
CONFIG -= app_bundle

CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp

#QMAKE_CXXFLAGS += -rdynamic

#QMAKE_CXXFLAGS += -fomit-frame-pointer
