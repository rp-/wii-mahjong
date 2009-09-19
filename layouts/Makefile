lyts := $(wildcard *.lyt)
hfiledep := ../mahjongglayouts.h
.PHONY: all

all: $(hfiledep)

$(hfiledep): $(lyts)
	python lyt2c.py $(lyts)
	mv mahjongglayouts.h mahjongglayouts.c ..

clean:
	rm -f $(hcfiledep)
