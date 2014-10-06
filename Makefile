QMAKE=qmake
MKDIR=mkdir -p
CD=cd
MAKE=make
CP=cp

.PHONY: all

all: lasm.pro
	$(MKDIR) build
	$(CD) build; $(QMAKE) ..
	$(MAKE) -C build
	$(MKDIR) bin
	$(CP) build/lasm-gui/bin/* ./bin
	$(CP) build/src/bin/* ./bin
