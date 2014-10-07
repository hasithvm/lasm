#-------------------------------------------------
#
# Project created by QtCreator 2013-04-02T21:41:42
#
#-------------------------------------------------

QT       += core gui
CONFIG   += debug_and_release

isEmpty(BUILDPREFIX) {
	BUILDPREFIX=.
}

OBJDIR = $$BUILDPREFIX/obj
DESTDIR = $$BUILDPREFIX/bin
OBJECTS_DIR = $$OBJDIR
MOC_DIR = $$OBJDIR/moc
UI_DIR = $$OBJDIR/ui
RCC_DIR = $$OBJDIR/rcc
Debug:TARGET = lasm-gui_debug
Release:TARGET = lasm-gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
SOURCES += main.cpp\
        qwmain.cpp

HEADERS  += qwmain.h

FORMS    += qwmain.ui

RESOURCES += lasm-gui.qrc


unix {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }
    BINDIR = $$PREFIX/bin

    INSTALLS += target

    target.path = $$BINDIR
}
win32:RC_FILE += ../res/lasm-gui.rc

macx:ICON = ../res/Lasm.icns
macx:QMAKE_POST_LINK += macdeployqt $$DESTDIR/$${TARGET}.app $$escape_expand(\\n\\t)

