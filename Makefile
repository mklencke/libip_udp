CC = gcc
INCLUDE = .
CFLAGS = -g -Wall
OBJECTS = inet.o ip.o
HEADERS = inet.h ip.h internal.h compat.h

default: test ip.o inet.o

test: test.c $(OBJECTS) $(HEADERS)
	$(CC) -I$(INCLUDE) $(CFLAGS) -o $@ $< $(OBJECTS)

ip.o: ip.c ip.h $(HEADERS)
	$(CC) -I$(INCLUDE) $(CFLAGS) -c -o $@ $<

inet.o: inet.c inet.h $(HEADERS)
	$(CC) -I$(INCLUDE) $(CFLAGS) -c -o $@ $<

clean:
	rm *.o
	rm test

