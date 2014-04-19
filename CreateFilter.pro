#-------------------------------------------------
#
# Project created by QtCreator 2014-03-31T22:31:15
#
#-------------------------------------------------

QT       -= core
QT       -= gui

TARGET = CreateFilter
DESTDIR = bin
OBJECTS_DIR = obj
CONFIG   += console
CONFIG   += c++11
CONFIG   -= app_bundle

QMAKE_MACOSX_DEPLOYMENT_TARGET=10.9

TEMPLATE = app

SOURCES += main.cpp \
    filter.cpp \
    vcprojectreader.cpp \
    filterwriter.cpp \
    vcfilterwriter.cpp \
    tinyxml/tinyxml2.cpp \
    uuid.cpp

HEADERS += \
    filter.h \
    vcprojectreader.h \
    filterwriter.h \
    vcfilterwriter.h \
    tinyxml/tinyxml2.cpp \
    StringUtil.h \
    uuid.h
