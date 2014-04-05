#-------------------------------------------------
#
# Project created by QtCreator 2014-03-31T22:31:15
#
#-------------------------------------------------

QT       += core


QT       -= gui

TARGET = CreateFilter
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    filter.cpp \
    vcprojectreader.cpp \
    filterwriter.cpp \
    vcfilterwriter.cpp

HEADERS += \
    filter.h \
    vcprojectreader.h \
    filterwriter.h \
    vcfilterwriter.h
