DOL := wiimahjongg.dol

.PHONY: all clean sound gfx layouts install

all: $(DOL)

sound:
	cd sound && $(MAKE)

gfx:
	cd gfx && $(MAKE)

layouts:
	cd layouts && $(MAKE)

$(DOL): sound gfx layouts
	$(MAKE) -f Makefile.DevKit

install: $(DOL)
	mv $< rpmahjongg/boot.dol

clean:
	cd sound && $(MAKE) clean
	cd gfx && $(MAKE) clean
	cd layouts && $(MAKE) clean
	$(MAKE) -f Makefile.DevKit clean

