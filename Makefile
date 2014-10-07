QMAKE=qmake
MKDIR=mkdir -p
CD=cd
MAKE=make
CP=cp
RM=rm -rf

.PHONY: all clean

all: lasm.pro
	$(MKDIR) build
	$(CD) build; $(QMAKE) ..
	$(MAKE) -C build all
	$(MKDIR) bin
	$(CP) build/lasm-gui/bin/* ./bin
	$(CP) build/src/bin/* ./bin

clean:
	$(MAKE) -C build clean
	$(RM) src/symtable-generated.hpp
	$(RM) src/p86asm.l
	${RM} build bin
