GCC = gcc
CFLAGS = -g -Wall -Wshadow

P2:	main.o scanner.o parser.o testTree.o
	$(GCC) $(CFLAGS) main.o scanner.o parser.o testTree.o -o P2

main.o:	main.c
	$(GCC) $(CFLAGS) -c main.c

testTree.o:	testTree.c
	$(GCC) $(CFLAGS) -c testTree.c testTree.h

parser.o:	parser.c parser.h
	$(GCC) $(CFLAGS) -c parser.c parser.h

scanner.o:	scanner.c scanner.h token.h
	$(GCC) $(CFLAGS) -c scanner.c scanner.h token.h

P2.o:	main.c
	$(GCC) $(CFLAGS) -c main.c

clean:
	rm *.o P2
