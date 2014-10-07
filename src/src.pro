isEmpty(BUILDPREFIX) {
	BUILDPREFIX=.
}

OBJDIR = $$BUILDPREFIX/obj
DESTDIR = $$BUILDPREFIX/bin
OBJECTS_DIR = $$OBJDIR
MOC_DIR = $$OBJDIR/moc
UI_DIR = $$OBJDIR/ui
RCC_DIR = $$OBJDIR/rcc

CONFIG += console debug_and_release
Debug:TARGET = lasm_debug
Release:TARGET = lasm
QMAKE_CXXFLAGS +=

TEMPLATE = app

macx {
	CONFIG -= app_bundle
}

SOURCES += symtable.cpp \
		data.cpp \
		data_immediate.cpp \
		common.cpp \
		Nodes.cpp \
		TextSegment.cpp \
		p86Assembler.cpp \
		preprocessor.cpp \
		lasm.cpp \
		VirgoWriter.cpp \
		ListingWriter.cpp \
		Logger.cpp

win32-msvc* {
    SOURCES += opcodes.dat
}
HEADERS  += TextSegment.hpp \
        Logger.hpp \
		IWriter.hpp \
		lasm.hpp \
		ListingWriter.hpp \
		common.h \
		data.h \
		Nodes.h \
		p86Assembler.h \
		preprocessor.h \
		symtable.h \
		version.h \
		VirgoWriter.h

OPCODESRC = opcodes.dat
opcode.name = Opcode Processor
opcode.input = OPCODESRC
opcode.output = p86asm.l
opcode.commands = python $$PWD/update-opcodes.py ${QMAKE_FILE_IN}
opcode.variable_out = GENERATED_FILES

symtable.name = Symtable Processor
symtable.input = OPCODESRC
symtable.output = symtable-generated.hpp
symtable.commands = echo Generating Symtable Output
symtable.depends = p86asm.l
symtable.variable_out = HEADERS

FLEXSRC = p86asm.l
flex.name = Flex Compiler
flex.input = FLEXSRC
flex.output = $$PWD/${QMAKE_FILE_BASE}.yy.cpp
flex.commands = flex -i -o ${QMAKE_FILE_OUT} $$PWD/${QMAKE_FILE_IN}
flex.variable_out = GENERATED_SOURCES
flex.depends = ${QMAKE_FILE_BASE}.tab.hpp p86asm.l

BISONSRC = p86asm.y
bison.name = Bison Compiler
bison.input = BISONSRC
win32-msvc2010 {
    bison.output = $$PWD/${QMAKE_FILE_BASE}.tab.cpp
} else {
    bison.output = ${QMAKE_FILE_BASE}.tab.cpp
}
bison.commands = bison -d -o ${QMAKE_FILE_OUT} $$OUT_PWD/${QMAKE_FILE_IN}
bison.variable_out = GENERATED_SOURCES
bison.depends = ${QMAKE_FILE_IN}

bisonHeader.name = Bison Header Compiler
bisonHeader.input = BISONSRC
bisonHeader.output = ${QMAKE_FILE_BASE}.tab.hpp
bisonHeader.commands = echo Generating Bison Results
bisonHeader.depends = ${QMAKE_FILE_BASE}.tab.cpp
bisonHeader.variable_out = GENERATED_HEADERS

QMAKE_EXTRA_COMPILERS += opcode symtable flex bison bisonHeader

unix {
    isEmpty(PREFIX) {
        PREFIX = /usr
    }
    BINDIR = $$PREFIX/bin

    INSTALLS += target

    target.path = $$BINDIR
}

