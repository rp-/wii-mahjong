PNGFILES := $(wildcard *.png)
JPGFILES := $(wildcard *.jpg)

CPNGFILES := $(PNGFILES:%.png=%.c)
HPNGFILES := $(PNGFILES:%.png=%.h)
CJPGFILES := $(JPGFILES:%.jpg=%.c)
HJPGFILES := $(JPGFILES:%.jpg=%.h)
HGFXFILES := $(HPNGFILES) $(HJPGFILES)
GFXFILES := $(HGFXFILES) $(CPNGFILES) $(CJPGFILES)

.PHONY :all clean

all: $(HGFXFILES)

%.h: %.png
	$(DEVKITPPC)/bin/raw2c $<

%.h: %.jpg
	$(DEVKITPPC)/bin/raw2c $<

clean:
	rm -f $(GFXFILES)

