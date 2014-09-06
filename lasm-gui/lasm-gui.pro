#-------------------------------------------------
#
# Project created by QtCreator 2013-04-02T21:41:42
#
#-------------------------------------------------

QT       += core gui

isEmpty(BUILDPREFIX) {
	BUILDPREFIX=.
}

OBJDIR = $$BUILDPREFIX/obj
DESTDIR = $$BUILDPREFIX/bin
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

unix {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }
    BINDIR = $$PREFIX/bin

    INSTALLS += target

    target.path = $$BINDIR
}
