GCC = gcc
CFLAGS = -g -Wall -Wshadow

P3:	main.o scanner.o parser.o testTree.o processIdent.o
	$(GCC) $(CFLAGS) main.o scanner.o parser.o testTree.o processIdent.o -o P3

main.o:	main.c
	$(GCC) $(CFLAGS) -c main.c

processIdent.o:	processIdent.c
	$(GCC) $(CFLAGS) -c processIdent.c processIdent.h

testTree.o:	testTree.c
	$(GCC) $(CFLAGS) -c testTree.c testTree.h

parser.o:	parser.c parser.h
	$(GCC) $(CFLAGS) -c parser.c parser.h

scanner.o:	scanner.c scanner.h token.h
	$(GCC) $(CFLAGS) -c scanner.c scanner.h token.h

P3.o:	main.c
	$(GCC) $(CFLAGS) -c main.c

clean:
	rm *.o P3
