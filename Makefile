# Copyright 2023 Echipa PCLP1

# compiler setup
CC=gcc
CFLAGS=-Wall -g

# define targets
TARGETS = bingo biblioteca

build: $(TARGETS)

bingo: bingo.c
	$(CC) $(CFLAGS) bingo.c -lm -o bingo

biblioteca: biblioteca.c
	$(CC) $(CFLAGS) biblioteca.c -o biblioteca

pack:
	zip -FSr 311CC_Ciurea_Ana-Sorina_Tema1.zip README Makefile *.c *.h

clean:
	rm -f $(TARGETS)

.PHONY: pack clean
