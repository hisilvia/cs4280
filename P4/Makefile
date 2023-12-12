GCC = gcc
CFLAGS = -g -Wall -Wshadow

comp:	main.o scanner.o parser.o testTree.o processIdent.o generator.o
	$(GCC) $(CFLAGS) main.o scanner.o parser.o testTree.o processIdent.o generator.o -o comp

main.o:	main.c
	$(GCC) $(CFLAGS) -c main.c

generator.o:	generator.c
	$(GCC) $(CFLAGS) -c generator.c generator.h

processIdent.o:	processIdent.c
	$(GCC) $(CFLAGS) -c processIdent.c processIdent.h

testTree.o:	testTree.c
	$(GCC) $(CFLAGS) -c testTree.c testTree.h

parser.o:	parser.c parser.h
	$(GCC) $(CFLAGS) -c parser.c parser.h

scanner.o:	scanner.c scanner.h token.h
	$(GCC) $(CFLAGS) -c scanner.c scanner.h token.h

comp.o:	main.c
	$(GCC) $(CFLAGS) -c main.c

clean:
	rm *.o comp
