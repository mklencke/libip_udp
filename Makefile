
CC = gcc
AR = ar
RANLIB = ranlib
INCLUDE = .
CFLAGS = -g -Wall
OBJECTS = inet.o ip.o
HEADERS = inet.h ip.h internal.h compat.h

all: libip.a libcn.a

test: test.c $(OBJECTS) $(HEADERS)
	$(CC) -I$(INCLUDE) $(CFLAGS) -o $@ $< $(OBJECTS)

libip.a: $(OBJECTS)
	$(AR) rv libip.a $(OBJECTS)
	$(RANLIB) libip.a

libcn.a: cn.o
	$(AR) rv libcn.a cn.o
	$(RANLIB) libcn.a

cn.o: cn.c

ip.o: ip.c ip.h $(HEADERS)
	$(CC) -I$(INCLUDE) $(CFLAGS) -c -o $@ $<

inet.o: inet.c inet.h $(HEADERS)
	$(CC) -I$(INCLUDE) $(CFLAGS) -c -o $@ $<

clean:
	-rm *.o
	-rm test
	-rm *.a

