QMAKE=qmake
MKDIR=mkdir -p
CD=cd
MAKE=make
CP=cp -r
RM=rm -rf
MV=mv

.PHONY: all clean

all: lasm.pro
	$(MKDIR) build
	$(CD) build; $(QMAKE) ..
	$(MAKE) -C build all
	$(MKDIR) bin
	$(CP) build/lasm-gui/bin/* ./bin
	$(CP) build/src/bin/* ./bin
ifeq (Darwin, $(shell uname -s))
	$(MV) ./bin/lasm ./bin/lasm-gui.app/Contents/MacOS/
	$(MV) ./bin/lasm_debug ./bin/lasm-gui_debug.app/Contents/MacOS/
endif

clean:
	$(MAKE) -C build clean
	$(RM) src/symtable-generated.hpp
	$(RM) src/p86asm.l
	${RM} build bin
