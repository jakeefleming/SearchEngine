/*
 * querier.c - Tiny Search Engine querier
 *
 * Search an index for urls that contain words
 * that the user queries for. Follow basic AND
 * OR logic that allows for more complex searches
 * 
 * Jake Fleming - 11/5/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../common/index.h"
#include "../common/pagedir.h"
#include "../libcs50/file.h"
#include "../libcs50/counters.h"

/************ Structs *************/

// hold two counters for intersect operation
typedef struct counterDuo {
    counters_t* srcCounter;
    counters_t* destCounter;
} counterDuo_t;

// hold document ID and score for ranking
typedef struct scorenode {
    int docID;
    int score;
} scorenode_t;

/************ Function Prototypes ************/
static void processQuery(char* line, index_t* index, const char* pageDirectory);
static bool parseQuery(char* line, char** words, int* wordCount);
static counters_t* processOrSequence(char** words, int start, int end, index_t* index);
static counters_t* processAndSequence(char** words, int start, int end, index_t* index);
static void counters_intersect(void* arg, const int key, int count);
static void counters_union(void* arg, const int key, int count);
static void printResults(counters_t* results, const char* pageDirectory);
static bool validateSyntax(char** words, int wordCount);
static void find_max_score_helper(void* arg, const int key, const int count);

/******************* main *******************/
/* handle arguemnts and call necessary functions */
int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./querier pageDirectory indexFilename\n");
        return 1;
    }

    // validate page directory
    const char* pageDirectory = argv[1];
    if (!pagedir_validate(pageDirectory)) {
        fprintf(stderr, "Error: %s is not a valid page directory.\n", pageDirectory);
        return 2;
    }

    // open index file
    const char* indexFilename = argv[2];
    FILE* indexFile = fopen(indexFilename, "r");
    if (indexFile == NULL) {
        fprintf(stderr, "Error: Could not open index file %s.\n", indexFilename);
        return 3;
    }

    // load index
    index_t* index = index_new(file_numLines(indexFile));
    if (index == NULL || !index_load(index, indexFile)) {
        fprintf(stderr, "Error: Could not load index from file %s.\n", indexFilename);
        fclose(indexFile);
        index_delete(index);
        return 5;
    }
    fclose(indexFile);

    // read and process queries
    char* line;
    printf("Query? ");
    while ((line = file_readLine(stdin)) != NULL) {
        processQuery(line, index, pageDirectory);
        printf("Query? ");
        free(line);
    }

    index_delete(index);
    return 0;
}

/******************* parseQuery *******************/
/* handle incorrect query inputs and normalize it */
static bool parseQuery(char* line, char** words, int* wordCount) {
    // convert line to lowercase and remove non-alphabetical characters
    char* src = line;
    char* dest = line;
    while (*src) {
        if (isalpha((unsigned char)*src)) {
            *dest++ = tolower((unsigned char)*src);  // convert to lowercase
        } else if (isspace((unsigned char)*src)) {
            *dest++ = ' ';  // spaces for tokenization
        }
        src++;
    }
    *dest = '\0';

    // tokenize query
    char* token = strtok(line, " ");
    *wordCount = 0;

    // allow for 100 words max
    while (token != NULL && *wordCount < 100) {
        words[(*wordCount)++] = token;
        token = strtok(NULL, " ");
    }

    return validateSyntax(words, *wordCount);
}


/******************* validateSyntax *******************/
/* handle incorrect and/or inputs accordint to specs */
static bool validateSyntax(char** words, int wordCount) {
    // empty query
    if (wordCount == 0) {
        return false;
    }

    // check for invalid sequences such as and/or at start or end
    if (strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0 ||
        strcmp(words[wordCount - 1], "and") == 0 || strcmp(words[wordCount - 1], "or") == 0) {
        fprintf(stderr, "Error: 'and'/'or' cannot be at the beginning or end.\n");
        return false;
    }

    // check for consecutive operators
    for (int i = 1; i < wordCount; i++) {
        if ((strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0) &&
            (strcmp(words[i - 1], "and") == 0 || strcmp(words[i - 1], "or") == 0)) {
            fprintf(stderr, "Error: 'and'/'or' cannot be consecutive.\n");
            return false;
        }
    }
    return true;
}

/*************** processQuery ***************/
/* call necessary functions to handle query */
static void processQuery(char* line, index_t* index, const char* pageDirectory) {
    char* words[100];
    int wordCount;

    // parse and validate query syntax
    if (!parseQuery(line, words, &wordCount)) {
        fprintf(stderr, "Error: Invalid query syntax.\n");
        return;
    }

    // process query with OR/AND logic
    // since AND takes presidence, call it within OR
    counters_t* results = processOrSequence(words, 0, wordCount, index);

    // nothing found 
    if (counters_size(results) == 0) {
        printf("No documents match.\n");
    } 
    // otherwise print the urls
    else {
        printResults(results, pageDirectory);
    }

    // cleanup 
    counters_delete(results);
}

/*************** processOrSequence ***************/
/* make unions of counters where OR separates queries */
static counters_t* processOrSequence(char** words, int start, int end, index_t* index) {
    // initialize final union pointer
    counters_t* combinedResults = counters_new();
    int i = start;

    while (i < end) {
        int j = i;
        // loop until we find an OR or reach end
        while (j < end && strcmp(words[j], "or") != 0) {
            j++;
        }
        // call AND helper on everything up until OR
        counters_t* andResult = processAndSequence(words, i, j, index);

        // take union of all the andResults
        counters_iterate(andResult, combinedResults, counters_union);

        // cleanup 
        counters_delete(andResult);
        i = j + 1;
    }
    return combinedResults;
}

/*************** processAndSequence ***************/
/* makes intersections of counters where AND (or nothing) separates queries */
static counters_t* processAndSequence(char** words, int start, int end, index_t* index) {
    // get the counter for the first word in query 
    counters_t* firstWordCounts = index_find(index, words[start]);
    // if no match, then entire intersection will be empty 
    if (firstWordCounts == NULL) {
        return counters_new();
    }

    // make pointer for result and copy firstWordCounts onto it 
    counters_t* result = counters_new();
    counters_iterate(firstWordCounts, result, counters_copy);

    // loop over provided AND sequence 
    for (int i = start + 1; i < end; i++) {
        // same thing, return empty counter if we ever find an empty one
        counters_t* wordCounts = index_find(index, words[i]);
        if (wordCounts == NULL) {
            counters_delete(result); 
            return counters_new();
        }
        // set up counterDuo for intersection
        counterDuo_t duo = { .srcCounter = wordCounts, .destCounter = result };
        counters_iterate(result, &duo, counters_intersect);
    }

    return result;
}

/*************** counters_intersect *******************/
/* helper for intersection operation in counters */
static void counters_intersect(void* arg, const int key, int count) {
    // initialize the struct and get the count of source
    counterDuo_t* duo = arg;
    int srcCount = counters_get(duo->srcCounter, key);

    // use smaller of two counts with conditional expression
    if (srcCount > 0) {
        counters_set(duo->destCounter, key, (srcCount < count) ? srcCount : count);
    }
    // otherwise, remove
    else {
        counters_set(duo->destCounter, key, 0);
    }
}

/*************** counters_union *******************/
/* union operation for counters, used in OR sequences */
static void counters_union(void* arg, const int key, const int count) {
    counters_t* dest_counters = arg;
    int current_count = counters_get(dest_counters, key);
    // sum the two counts
    counters_set(dest_counters, key, current_count + count);
}

/*************** find_max_score_helper ***************/
/* helper function to find the max score for printing results */
void find_max_score_helper(void* arg, const int key, const int count) {
    scorenode_t* max_node = arg;
    if (count > max_node->score) {
        max_node->score = count;
        max_node->docID = key;
    }
}

/*************** printResults ***************/
/* display hits in order of relevance */
static void printResults(counters_t* results, const char* pageDirectory) {
    printf("Matches %d documents (ranked):\n", counters_size(results));

    // loop through all word keys
    while (counters_size(results) > 0) {
        scorenode_t max_node = { .docID = 0, .score = 0 };
        counters_iterate(results, &max_node, find_max_score_helper);

        // no valid documents left
        if (max_node.score == 0) break;  

        // get URL if there are any
        char* url = getDocumentURL(pageDirectory, max_node.docID);
        if (url != NULL) {
            printf("score %d doc %d: %s\n", max_node.score, max_node.docID, url);
            free(url);  // free the URL retrieved from file
        }

        counters_set(results, max_node.docID, 0); // remove
    }
}