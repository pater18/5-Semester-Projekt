TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    lidarsensor.cpp \
    marbledetection.cpp

CONFIG += link_pkgconfig
PKGCONFIG += gazebo
PKGCONFIG += opencv4


unix: PKGCONFIG += fuzzylite

HEADERS += \
    callBackFunctions.h \
    fuzzycontroller.h \
    lidarsensor.h \
    marbledetection.h
