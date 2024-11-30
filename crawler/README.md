# CS50 Tiny Search Engine

## CS50 Fall 2024

### Crawler
A `crawler` is a module that searches for links within HTML code, recursively adding every URL it visits to a pageDirectory at a given depth so that they can later be accessed by our tiny search engine.

### Usage
The *crawler* module takes three parameters, `seedURL pageDirectory maxDepth`, which reference the string for the root URL we will start the crawl from, the string to the path of where the pageDirectory exists, and an integer value between 0 and 10 for how far we want to crawl.

### Implementation
See the implementation spec for details here. Also note that I have added a fuction `void dummyDelete(void* item) {return;}` that I pass into hashtable_delete to fix a bug I was having with the sets in my hashtable not being freed.

### Assumptions
No assumptions beyond those that are clear from the spec.

### Files
- `Makefile` - compilation procedure
- `crawler.c` - the implementation
- `testing.sh` - the script that tests all cases
- `testing.out` - result of `make test` in the crawler directory
- `README.md` - important notes left out in specs
- `REQIREMENTS.md` - requirement spec provided in CS50
- `DESIGN.md` - design spec provided in CS50
- `IMPLEMENTATION.md` - implementation spec provided in CS50

### Compilation
To compile, simply run `make` in the main TSE directory. This will compile all the necessary .o files and the common library.

### Testing
To test, simply `make test` in the crawler directory. See `testing.out` for details of testing and example output.

This test inclues not only boundary cases but also valgrind tests to ensure no memory leaks occur.