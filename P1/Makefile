GCC = gcc
CFLAGS = -g -Wall -Wshadow

P1:     main.o scanner.o
        $(GCC) $(CFLAGS) main.o scanner.o -o P1

main.o: main.c
        $(GCC) $(CFLAGS) -c main.c

scanner.o:      scanner.c scanner.h token.h
        $(GCC) $(CFLAGS) -c scanner.c scanner.h token.h

P1.o:   main.c
        $(GCC) $(CFLAGS) -c main.c

clean:
        rm *.o P1
