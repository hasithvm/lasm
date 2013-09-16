CP:=cp
MKDIR:=mkdir -p
QMAKE:=qmake
RM:=rm

.PHONY: all
.PHONY: clean


all: lasm gui

lasm:
	$(MAKE) -C src/ all
	$(MKDIR) bin/
	$(CP) src/lasm bin/lasm

gui:
	$(QMAKE) -makefile -o lasm-gui/Makefile lasm-gui/lasm-gui.pro
	$(MAKE) -C lasm-gui/ all
	$(CP) lasm-gui/lasm-gui bin/lasm-gui

clean:
	$(MAKE) -C src/ clean
	$(MAKE) -C lasm-gui/ clean
	$(RM) bin/*

