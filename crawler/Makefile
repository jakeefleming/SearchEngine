# Makefile for 'crawler' module
#
# Jake Fleming, 10/21/2024

OBJS = crawler.o
LIBS = ../libcs50/libcs50.a ../common/common.a
EXEC = crawler

# Uncomment the following to turn on verbose memory logging
# TESTING = -DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50 -I../common
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# build the crawler executable
$(EXEC): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(EXEC)

# object file compilation rules
crawler.o: crawler.c ../libcs50/webpage.h ../common/pagedir.h ../libcs50/hashtable.h ../libcs50/bag.h
	$(CC) $(CFLAGS) -c crawler.c

# ensure the common library is built before linking
../common/common.a:
	$(MAKE) -C ../common

.PHONY: test valgrind clean

test: $(EXEC)
	./testing.sh &> testing.out

valgrind: $(EXEC)
	$(VALGRIND) ./$(EXEC)

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(EXEC)