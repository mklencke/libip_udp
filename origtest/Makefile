CC = gcc
INCLUDE = ..
CFLAGS = -g -Wall
OBJECTS = ../inet.o ../ip.o

default: client server

client: client.c $(OBJECTS)
	$(CC) -I$(INCLUDE) -o $@ $< $(OBJECTS)

server: server.c $(OBJECTS)
	$(CC) -I$(INCLUDE) -o $@ $< $(OBJECTS)

clean:
	rm client
	rm server

