CC = gcc
INCLUDE = ..
CFLAGS = -g -Wall
LIBS = ../libip.a

default: xmllog

xmllog: xmllog.c $(LIBS)
	$(CC) -I$(INCLUDE) -o $@ $< $(LIBS)

clean:
	rm xmllog

