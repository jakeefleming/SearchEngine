# Makefile for 'querier' module
#
# Jake Fleming, 11/4/24

OBJS = querier.o
LIBS = ../common/common.a ../libcs50/libcs50.a
EXEC = querier

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
CC = gcc
MAKE = make

# build the querier executable
all: $(EXEC)

# build the querier executable
querier: querier.o $(LIBS)
	$(CC) $(CFLAGS) querier.o $(LIBS) -o querier

# object file compilation rules
querier.o: querier.c ../common/index.h ../common/pagedir.h ../libcs50/counters.h
	$(CC) $(CFLAGS) -c querier.c

# ensure the common and libcs50 libraries are built before linking
../common/common.a:
	$(MAKE) -C ../common

../libcs50/libcs50.a:
	$(MAKE) -C ../libcs50

.PHONY: test valgrind clean

# Run tests with test script
test: querier
	./testing.sh &> testing.out

# Run valgrind tests for memory leaks
valgrind: querier
	valgrind --leak-check=full --show-leak-kinds=all ./querier ../data/toscrape ../data/toscrape.index

# Clean up generated files
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(EXEC)
