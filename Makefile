CP:=cp
MKDIR:=mkdir -p

lasm:
	$(MAKE) -C src/ all
	$(MKDIR) bin/
	$(CP) src/lasm bin/lasm
