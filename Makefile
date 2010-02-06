# Makefile.  Generated from Makefile.in by configure.

SOURCES = main.c

CFLAGS = -Wall -std=c99 -g -O0

# prefix related substituion variables
#PREFIX=/usr/local
PREFIX = /usr/local

cacampc: $(SOURCES)
	gcc $(CFLAGS) -o $@ $+ `pkg-config --libs --cflags caca libmpdclient`

install:
	install -d $(PREFIX)/bin
	install -m 0755 cacampc $(PREFIX)/bin

clean:
	rm cacampc
