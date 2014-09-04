QMAKEVERSION = $$[QMAKE_VERSION]
ISQT4 = $$find(QMAKEVERSION, ^[2-9])
isEmpty( ISQT4 ) {
	error("Use qmake from Qt v4.4 or higher.")
}

lasm.target = lasm
lasm.commands = make -C $$PWD/src/ OBJDIR=$$OUT_PWD/obj BINDIR=$$OUT_PWD/bin

QMAKE_EXTRA_TARGETS += lasm
PRE_TARGETDEPS += lasm

TEMPLATE = subdirs
SUBDIRS = lasm-gui
