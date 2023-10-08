GCC = gcc
CFLAGS = -g -Wall -Wshadow

P0:     p0.o tree.o
        $(GCC) $(CFLAGS) p0.o tree.o -o P0

p0.o:   p0.c
        $(GCC) $(CFLAGS) -c p0.c

tree.o: tree.c tree.h
        $(GCC) $(CFLAGS) -c tree.c tree.h

P0.o:   P0.c
        $(GCC) $(CFLAGS) -c P0.c

clean:
        rm *.o P0
