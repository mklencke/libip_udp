CC = gcc
INCLUDE = ..
CFLAGS = -g -Wall
LIBS = ../libip.a

default: client server

client: client.c $(LIBS)
	$(CC) -I$(INCLUDE) -o $@ $< $(LIBS)

server: server.c $(LIBS)
	$(CC) -I$(INCLUDE) -o $@ $< $(LIBS)

clean:
	rm client
	rm server

