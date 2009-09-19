WAVFILES := $(wildcard *.wav)
RAWFILES := $(WAVFILES:%.wav=%.raw)
MODFILES := $(wildcard *.mod)
CFILES := $(WAVFILES:%.wav=%.c)
HFILES := $(WAVFILES:%.wav=%.h)
CFILES += $(MODFILES:%.mod=%.c)
HFILES += $(MODFILES:%.mod=%.h)
.PHONY :all clean install

all: $(HFILES)

%.h: %.raw 
	raw2c $<

%.h: %.mod
	raw2c $<

%.raw: %.wav 
	sox -V $< -c 1 --bits 8 -B -s $@
#	sox -V $< -c 1 --bits 8 -B -s $@ vol 2

clean:
	rm -f $(CFILES) $(HFILES)
