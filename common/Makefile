# Makefile for 'common' module
#
# Jake Fleming, 10/20/2024

# object files to be compiled into the library
OBJS = pagedir.o index.o word.o
LIBS = ../libcs50/libcs50.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
CC = gcc
AR = ar
ARFLAGS = rcs

# build common.a as the static library from object files
common.a: $(OBJS)
	$(AR) $(ARFLAGS) common.a $(OBJS) $(LIBS)

# compile pagedir.o
pagedir.o: pagedir.c pagedir.h
	$(CC) $(CFLAGS) -c pagedir.c

# compile index.o
index.o: index.c index.h
	$(CC) $(CFLAGS) -c index.c

# compile word.o
word.o: word.c word.h
	$(CC) $(CFLAGS) -c word.c

# ensure libcs50 is built before building common
../libcs50/libcs50.a:
	$(MAKE) -C ../libcs50

# clean target
clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f common.a
	rm -f core