CC = gcc
INCLUDE = ..
CFLAGS = -g -Wall
LIBS = ../libip.a

default: simplelog

simplelog: simplelog.c $(LIBS)
	$(CC) -I$(INCLUDE) -o $@ $< $(LIBS)

clean:
	rm simplelog

