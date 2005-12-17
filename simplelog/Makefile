CC = gcc
INCLUDE = ..
CFLAGS = -g -Wall
LIBS = ../libip.a

default: simplelog

simplelog: simplelog.c $(LIBS)
	$(CC) $(CFLAGS) -I$(INCLUDE) -o $@ $< $(LIBS)

clean:
	rm simplelog

