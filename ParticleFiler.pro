TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        particlefilter.cpp

CONFIG += link_pkgconfig

PKGCONFIG += opencv4

HEADERS += \
    landmarks.h \
    particlefilter.h
