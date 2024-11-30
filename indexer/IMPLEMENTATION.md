# CS50 TSE Indexer
## Implementation Spec

Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We use two data structures: a hashtable (which contains sets) and counters.
We crate a struct index that holds a hashtable full of counters. Where the keys to the hashtable are words and the items are counters.
Further, the key to the counter is a docID, and the counter counts how many times the word has appeared in the particular document.
The size of the hashtable (slots) is impossible to determine in advance, so we use 500.

`index_new` allocates and initializes an index_t structure with a specified number of slots
`index_insert` retrieves the counters_t for a word, if it doesn't exist, creates one, and otherwise increments the count for the docID
`index_find` simply calls `hashtable_find` on a word and returns the counter
`index_save` uses a helper function to iterate over every word in the hashtable and then another helper function to iterate over every counter for each word. This saves each index in the following format:
```
word docID count docID count ...
```
`index_load` reads words, docIDs, and counts, calling `index_insert` to build an actual index 
`index_delete` deletes every counter and then finally the hashtable


## Control flow

The Indexer is implemented in one file `indexer.c`, with three functions.

### main

The `main` function simply parses the arguments, calls `indexBuild`, `index_save`, and `index_delete`, then exits zero.

For parsing arguments, given arguments from the command line, extract them into the function parameters; return only if successful.

* for `pageDirectory`, call `pagedir_validate()` from `pagedir.c` that ensures we can access the directory
* for `indexFilename`, ensure we can open the file
* if any trouble is found, print an error to stderr and exit non-zero.

### indexBuild

Do the real work of indexer

Pseudocode:

	instantiate a new index of size 500 and docID to 1
    while the webpage we load in is not null (see pagedir_load)
        call indexPage on the webpage
        increment docID
        and delete the webpage
    return index

### indexPage

This function implements the *pagescanner*
Given a `webpage`, scan the given page to extract any words to add to the index
Pseudocode:

	while there's another word to get
        if the length of the word is >= 3
            normalize the word
            insert it into index
        free the word

## Other modules

### pagedir

We create a re-usable module `pagedir.c` to handles the *pagesaver*  mentioned in the design (writing a page to the pageDirectory), and marking it as a Crawler-produced pageDirectory (as required in the spec).
We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about how to initialize and validate a pageDirectory, and how to write and read page files, in one place. See the IMPLEMENTATION.md spec in crawler for details on `pagedir_init` and `pagedir_save`.

Pseudocode for `pagedir_validate`

    check if we can access the directory using stat and S_ISDIR
    allocate space for the path to the .crawler file
    if the directory doesn't contain the .crawler file
        return false
    return true

Pseudocode for `pagedir_load`

    construct the filepath for the document
    open the file
    read the url, depth, and html using the methods in file.c
    create a webpage object with this information and return to caller

### word

We create a module `word.c` to allow us to normalize a word to all lowercase. See `word.h` for more details

Pseudocode for `normalizeWord`

    if word is null
        return null
    for char in word
        if char is alphabetical
            call tolower on char
    
### libcs50

We leverage the modules of libcs50, most notably `hashtable`, `counters`, `webpage`, and `file`.
See that directory for module interfaces.

## Function prototypes

### indexer

```c
int main(int argc, char* argv[]);
static index_t* indexBuild(char* pageDirectory);
static void indexPage(index_t* index, webpage_t* page, const int docID)
```

### index

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `index.h` and is not repeated here.

```c
index_t* index_new(const int num_slots);
bool index_insert(index_t* index, const char* word, int docID);
void* index_find(index_t* index, const char* word);
bool index_save(index_t* index, FILE* fp);
bool index_load(index_t* index, FILE* fp);
void index_delete(index_t* index);
```

### pagedir

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_load(const char* pageDirectory, int docID);
```

### word

```c
void normalizeWord(char* word);
```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

All code uses defensive-programming tactics to catch and exit e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

We create a test `indextest.c` to simply read through an old index and create a new index using `index_load` and `index_save`

Pseudocode

    ensure correct number of arguments
    open the old index for reading
    create a new index with index_new and 500 slots
    call index_load on the old index into the new index
    close the old index
    open the new index for writing
    save the index into the new index file
    delete the index 
    close the new index file
    return 0

### Integration/system testing

We write a script `testing.sh` that invokes the indexer several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, a run of index on three different pageDirectories: letters-5, toscrape-1, and wikipedia-1
Third, a run of indextest on each of the three new index files created, calling `indexcmp` to ensure they are the same/
Fourth, another `indexcmp` check that my index file matches that which was provided
Finally, a run of both `index` and `indextest` using Valgrind to ensure there are no memory leaks
Run that script with `make test` from the indexer direcory
Verify correct behavior by studying the output in `testing.out`