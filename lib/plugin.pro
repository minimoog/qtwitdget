TEMPLATE = lib
CONFIG += qt plugin
QT += declarative sql
TARGET = tweetlistmodel

INCLUDEPATH += ../
OBJECTS_DIR = tmp
MOC_DIR = tmp
DESTDIR = ./

HEADERS += tweetqmllistmodel.h \
           tweetlistmodelplugin.h 
		   
SOURCES += tweetqmllistmodel.cpp \
           tweetlistmodelplugin.cpp
		   
		   