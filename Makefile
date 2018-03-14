DOL := wii-mahjong.dol
INSTALLFOLDER := mahjongg
INSTALLDOL := $(INSTALLFOLDER)/boot.dol

.PHONY: all clean sound gfx layouts install 7zip zip

all: $(DOL)

sound:
	cd sound && $(MAKE)

gfx:
	cd gfx && $(MAKE)

layouts:
	cd layouts && $(MAKE)

$(DOL): sound gfx layouts
	$(MAKE) -f Makefile.DevKit

$(INSTALLDOL): $(DOL)
	mv $< $(INSTALLDOL)

install: $(INSTALLDOL)

7zip: $(INSTALLDOL)
	7z a $(INSTALLFOLDER).7z $(INSTALLFOLDER)

zip: $(INSTALLDOL)
	zip $(INSTALLFOLDER).zip $(INSTALLFOLDER)/*

cls:
	rm -f *.dol *.elf
clean:
	cd sound && $(MAKE) clean
	cd gfx && $(MAKE) clean
	cd layouts && $(MAKE) clean
	$(MAKE) -f Makefile.DevKit clean
	rm -Rf $(INSTALLFOLDER).zip $(INSTALLFOLDER).7z $(INSTALLDOL)
	rm -f mahjongglayouts.c mahjongglayouts.h
run: $(DOL)
	wiiload $(DOL)
rebuild: sound gfx layouts
	rm -f build/*.o
	$(MAKE) -f Makefile.DevKit

