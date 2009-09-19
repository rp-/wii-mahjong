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
	raw2c $<

%.h: %.jpg
	raw2c $<

clean:
	rm -f $(GFXFILES)

