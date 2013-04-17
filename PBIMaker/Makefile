VERSION=1.3
PREFIX?=/usr/local
CC=gcc
MODULES = pbi.o main.o

all: $(MODULES)
	$(CC) -o pbimaker $(MODULES)

clean:
	rm -f *.o
	rm -f pbimaker

$(MODULES): Makefile

pbi.o: pbi.c pbi.h
	$(CC) -c pbi.c -DPREFIX=\"$(PREFIX)\"

main.o: main.c pbi.h
	$(CC) -c main.c

install: pbimaker
	cp pbimaker $(PREFIX)/bin/
	mkdir -p $(PREFIX)/share/pbimaker
	cp defaulticon.png $(PREFIX)/share/pbimaker/

deinstall:
	rm $(PREFIX)/bin/pbimaker
	rm -rf $(PREFIX)/share/pbimaker

