# Search Engine

## Fall 2024

### Querier
An `querier` is a module that searches for word requests in an index file, gathering the documents they are found in and printing out those webpage URLs from the pageDirectory to the user. It handles logic with AND and OR requests so you can do more complex web searches.

### Usage
The *querier* module takes two parameters, `./querier pageDirectory indexFilename`, which reference the string to the path of where the pageDirectory exists and a string to the filename we want to write the index into.

### Implementation
See the implementation spec for details here. 

### Assumptions
The user's query must be no longer than 100 words including "and" and "or"

### Files
- `Makefile` - compilation procedure
- `querier.c` - the implementation
- `testing.sh` - the script that tests all cases
- `testing.out` - result of `make test` in the querier directory
- `README.md` - important notes left out in specs
- `IMPLEMENTATION.md` - implementation spec 
- `DESIGN.md` - design spec

### Compilation
To compile, simply run `make` in the main TSE directory. This will compile all the necessary .o files and libraries.

### Testing
To test, simply `make test` in the querier directory. See `testing.out` for details of testing and example output.

This test inclues not only boundary cases but also valgrind tests to ensure no memory leaks occur.
