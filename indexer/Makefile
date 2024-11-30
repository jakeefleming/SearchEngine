# Makefile for 'indexer' module
#
# Jake Fleming, 10/28/24

OBJS = indexer.o indextest.o
LIBS = ../common/common.a ../libcs50/libcs50.a
EXEC = indexer indextest

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50 -I../common
CC = gcc
MAKE = make

# build both executables
all: $(EXEC)

# build the indexer executable
indexer: indexer.o $(LIBS)
	$(CC) $(CFLAGS) indexer.o $(LIBS) -o indexer

# build the indextest executable
indextest: indextest.o $(LIBS)
	$(CC) $(CFLAGS) indextest.o $(LIBS) -o indextest

# object file compilation rules
indexer.o: indexer.c ../common/index.h ../libcs50/counters.h ../common/pagedir.h
	$(CC) $(CFLAGS) -c indexer.c

indextest.o: indextest.c ../common/index.h
	$(CC) $(CFLAGS) -c indextest.c

# ensure the common and libcs50 libraries are built before linking
../common/common.a:
	$(MAKE) -C ../common

../libcs50/libcs50.a:
	$(MAKE) -C ../libcs50

.PHONY: test valgrind clean

test: indexer indextest
	./testing.sh &> testing.out

valgrind: indexer indextest
	valgrind --leak-check=full --show-leak-kinds=all ./indexer ../data/letters ../data/letters.index
	valgrind --leak-check=full --show-leak-kinds=all ./indextest ../data/letters.index ../data/letters_test.index

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f indexer indextest
	rm -f ../data/*.index ../data/*_test.index