# CS50 TSE Querier
## Implementation Spec

Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We use one main data structure for the querier: counters. See counters.h for more details. For the querier, I implemented two new methods:

```c
int counters_size(counters_t* ctrs);
void counters_copy(void* arg, const int key, const int count);
```

`counters_size` is useful in the implementation becasue when we are printing out all of the keys from our result counter to the user, we want to do it until we've removed every key to the output.
`counters_copy` is a helper function for counters_iterate that I used to handle memory issues I was having with intersect, copying the counterset from our first word retrieval onto a result we will do repeated intersections on

I also have two additional structs that were useful for minor parts of the code.

`counterDuo` wraps two counters together which was another piece of debugging I included for the intersection process.
`scorenode` wraps a docID and score count together which is useful for sorting the results during the output print

## Control Flow

The querier is implemented in one file `querier.c` with 10 functions.

### Main

The `main` function simply parses the arguments and calls `processQuery` on loop until the EOF on stdin is reached.

For parsing arguments, given arguments from the command line, extract them into the function parameters; return only if successful.

* for `pageDirectory`, call `pagedir_validate()` from `pagedir.c` that ensures we can access the directory
* for `indexFilename`, ensure we can open the file and call index_load without error.
* if any trouble is found, print an error to stderr and exit non-zero.

### Process Query

In `processQuery`, we are calling `parseQuery` and `processOrSequence` on every query. `processOrSequence` will return us the result counterset of the query and we can check the size of it with our counter helper, returning either "No documents" if it is 0, or calling `printResults` otherwise.

### Parse Query

The `parseQuery` function begins by converting all capital letters to lowercase and removing non alphabet characters. Then, it tokenizes each query into a list of words and calls `validateSyntax` on this list.

### Validate Syntax

Here we ensure 3 main things. 

1. The query is not empty
2. The query does not begin or end with "and" or "or"
3. There are no consecutive "and" or "or" placements

The psuedocode for this is as follows:

    if wordCount = 0
        return false
    if the first word or last word is "and" or "or"
        return false
    for every word in the word list
        if the word is "and" or "or" and so is the word before it
            return false
    return true
    
### Process Or Sequence

This method begins the start of our AND/OR logic. We begin with "or" because "and" takes presidence, so it will happen within this function. Here is the pseudocode:

    initialize a result counterset
    i = start
    while i < end
        j = i
        while j < end or words[j] equals "or"
            increment j
        call processAndSequence
        union the andResult with the main result
        delete the andResult
        increment i to j + 1 to start next or sequence
    return the combined result

### Process And Sequence

This method takes every internal piece that was separated by the start, end, and other "or" calls. 
Here is the pseudocode

    get the counter for the first word in this part of query
    if it is null
        return an empty counterset
    initialize a result counterset
    copy the first word's counter onto it
    loop i from start + 1 to end
        find the counterset at words[i]
        once again if it is null
            return an empty counterset
        wrap the two counters in a countersDuo struct
        intersect them
    return result

### Counters Intersect

A helper function for counters_iterate that takes a counterDuo as an argument and loops over all of the keys and values of another counter, combining it into one destCounter that has the lower score of the two words. This works very straight forward with the use of a simple conditional function that takes the min of the two counts: `(srcCount < count) ? srcCount : count`. If one of the counters does not have the word, we simply update the count to 0.

### Counters Union

Just like intersect, this is another helper function that essentially appends two counters together. Summing the counts for every word works because counters_get is predefined to return 0 if it does not find the word.

### Print Results

This function takes our final result counter from `processQuery` and prints every document that matched the query we handled. 
Here is the pseudocode:

    print the number of documents found
    while the counter_size > 0
        create a new scorenode
        update the scorenode to the max in the counter with helper function
        if it is 0
            break
        otherwise get the url
        if it is not null
            print the url
            free the url
        set the count to 0 for this node (essentially removing it from counter)
        
### Find Max Score Helper

Once again another helper for counters_iterate that will update a scorenode to the key item pair that has the highest count. Simply check if count is greater than maxnode->score and update if it is.

## Testing Plan

Here is an implementation-specific testing plan.

### Integration/System Testing

We write a script `testing.sh` that invokes the querier several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, a run of several different queries on toscrape-2, some of which are invalid
Finally, we run a similar valgrind test on 2 of these valid queries to show no memory leaks
Run that script with `make test` from the querier direcory
Verify correct behavior by studying the output in `testing.out`