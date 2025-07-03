# Search Engine

## Fall 2024

### Indexer
An `indexer` is a module that reads documents from a pageDirectory (created by the crawler) and adds words to an index data structure that counts the number of times a word shows up in each document. 

### Usage
The *indexer* module takes two parameters, `./indexer pageDirectory indexFilename`, which reference the string to the path of where the pageDirectory exists and a string to the filename we want to write the index into.

### Implementation
See the implementation spec for details here. 

### Assumptions
No assumptions beyond those that are clear from the spec.

### Files
- `Makefile` - compilation procedure
- `indexer.c` - the implementation
- `indextest.c` - a direct test on the index data structure methods
- `testing.sh` - the script that tests all cases
- `testing.out` - result of `make test` in the crawler directory
- `README.md` - important notes left out in specs
- `IMPLEMENTATION.md` - implementation spec (design and requirements in the CS50 tse instructions)

### Compilation
To compile, simply run `make` in the main TSE directory. This will compile all the necessary .o files and the common library.

### Testing
To test, simply `make test` in the crawler directory. See `testing.out` for details of testing and example output.

This test inclues not only boundary cases but also valgrind tests to ensure no memory leaks occur.
