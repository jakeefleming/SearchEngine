# CS50 TSE Querier
## Design Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and focus on the implementation-independent design decisions.
We will focus on the follow:

- User interface
- Inputs and outputs
- Functional decomposition into modules
- Logic/Algorithm Flow
- Major data structures
- Testing plan

## User interface

As described in the [Requirements Spec](REQUIREMENTS.md), the querier's only interface with the user is on the command-line; it must always have two arguments.

```bash
$ querier pageDirectory indexFilename
```

For example, to query an preprovided paegeDirectory and indexFilename, we might use a command such as the following:

``` bash
$ ./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index
```

## Inputs and outputs

*Input:* there are no file inputs; there are command-line parameters described above.

*Output:* Per the requirements spec, the Querier will sort all the documents that are found with a given query, and output to the user the following messages, sorting the documents top to bottom by the number of hits the query had in the document

```
Query? Matches 24 documents (ranked):
score 11 doc 477: http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/fifty-shades-darker-fifty-shades-2_892/index.html
score 7 doc 193: http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/fifty-shades-freed-fifty-shades-3_156/index.html
score 5 doc 456: http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/grey-fifty-shades-4_592/index.html
score 1 doc 476: http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/the-wedding-dress_864/index.html
...
```

## Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules
 2. *processQuery*, which runs all of the necessary methods on each query
 3. *parseQuery*, which tokenizes and lowercases the query
 4. *validateQuery*, which ensures the logic is correct
 5. *processOrSequence*, which unionizes all of the separate AND sequences
 6. *processAndSequence*, which intersects all the words not seprated by OR
 7. *printResults*, which prints the documents out in order of number of hits

And some helper modules

 1. *counters_intersect*, which sets the result counter to the min value of every key between the two
 2. *counters_union*, which sets the result counter to the sum of counts of every key between the two
 3. *counters_size*, which returns the number of key-count pairs in the conuterset
 4. *counters_copy*, which copies one counter into another
 5. *find_max_score_helper*, which updates a scorenode to the one with the max count

## Pseudo code for logic/algorithmic flow

The querier will run as follows:

    parse the command line, validating the pageDirectory and indexFile
    for each query provided by the user in terminal
        make an empty list of words
        lowercase and tokenize the query into the list
        if the query is invalid
            return error
        create a new counterset orResult
        while i (first pointer) < end
            loop over every word until OR is reached or the end with a second pointer j
            get the counterset from the index of the word at the first pointer
            for word between i and j
                intersect its counterset with the first one
            union the the andResult with the orResult
        if counter is empty print "No documents match"
        otherwise 
            print the size of the result counterset
            while the counterset is not empty
                find the max scorenode
                get the URL
                print the URL
                remove the scorenode from the counterset

The querier completes and exits when the user has entered ^d, the end of file, meaning they have no more queries

## Major data structures

Helper modules provide all the data structures we need:

- *counter* of docIDs
- *pageDirectory* with methods for operating on directories
- *index* with methods for gathering the counters from a hashtable
- *file* with methods for operating on files

## Testing plan

For my testing, I ran it all on toscrape-2 which has many websites and a very large index of words. A serious of normal and fuzz testing demonstrates proper handling of all error cases and successful execution of simple and complex queries. See the results of testing.sh in testing.out, and more specific descriptions in the implementation spec.