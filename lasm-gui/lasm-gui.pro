#-------------------------------------------------
#
# Project created by QtCreator 2013-04-02T21:41:42
#
#-------------------------------------------------

QT       += core gui

OBJDIR = ../obj
DESTDIR = ../bin
OBJECTS_DIR = $$OBJDIR
MOC_DIR = $$OBJDIR/moc
UI_DIR = $$OBJDIR/ui
RCC_DIR = $$OBJDIR/rcc
TARGET = lasm-gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

SOURCES += main.cpp\
        qwmain.cpp

HEADERS  += qwmain.h

FORMS    += qwmain.ui


RESOURCES += lasm-gui.qrc
