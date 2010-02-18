# Makefile.  Generated from Makefile.in by configure.

# tool related substitution variables
CC = gcc
LIBS = -lmpdclient -lcaca 
#CFLAGS = -g -O2
CFLAGS = -Wall -std=c99 -g -O0 -I/usr/local/include -L/usr/local/lib
#$(CC) $(CFLAGS) -o $@ $+ `pkg-config --libs --cflags caca libmpdclient`

SOURCES = main.c


# prefix related substituion variables
#PREFIX=/usr/local
PREFIX = /usr/local

cacampc: $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $+ $(CFLAGS) $(LIBS)

install:
	install -d $(PREFIX)/bin
	install -m 0755 cacampc $(PREFIX)/bin

clean:
	rm cacampc
