CFLAGS = -Wall -std=c99 -g

cacampc: main.c
	gcc $(CFLAGS) -o $@ $+ `pkg-config --libs --cflags caca libmpdclient`

clean:
	rm cacampc
