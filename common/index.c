/*
 * index.c - Tiny search engine index data structure
 *
 * Creates an index struct that is a hashtable of
 * counters for words
 * 
 * Jake Fleming - 10/28/24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "word.h"
#include "index.h"

/**************** local types ****************/

struct index {
    hashtable_t* hashtable;
};

/**************** local functions ****************/

/****************** print_counter ******************/
/* print the docID and count of word for every file*/
static void
print_counter(void* arg, const int key, int count)
{
    FILE* fp = (FILE*)arg;
    if (fp != NULL && count > 0) {
        // print docID and count
        fprintf(fp, " %d %d", key, count);
    }
}


/**************** index_save_helper ****************/
/* print the word then iterate through the counter*/
static void
index_save_helper(void* fp, const char* key, void* item)
{
    FILE* outputFile = (FILE*) fp;
    counters_t* counter = (counters_t*) item;

    if (key != NULL && counter != NULL) {
        // print word
        fprintf(outputFile, "%s", key);

        // iterate through counter and print docID and count
        counters_iterate(counter, outputFile, print_counter);

        fprintf(fp, "\n");
    }
}

/*********** index_delete_helper ***********/
/* delete the counters in the index */
static void
index_delete_helper(void* item) 
{
    counters_t* counter = item;
    counters_delete(counter);
}

/**************** global functions ****************/


/**************** index_new ****************/
/* see index.h header for more details */
index_t*
index_new(const int num_slots)
{
    index_t* index = malloc(sizeof(index_t));

    // error allocating space
    if (index == NULL) {
        return NULL;
    }

    // instantiate the hashtable
    index->hashtable = hashtable_new(num_slots);

    // error allocating hashtable
    if (index->hashtable == NULL) {
        free(index);
        return NULL;
    }
    return index;
}

/**************** index_insert ****************/
/* see index.h header for more details */
bool
index_insert(index_t* index, const char* word, int docID)
{
    if (index == NULL || word == NULL || docID < 1) {
        return false;
    }

    // Find the counters for the word
    counters_t* counter = hashtable_find(index->hashtable, word);
    if (counter == NULL) {
        // If the word isn't in the hashtable, create a new counter for it
        counter = counters_new();
        if (counter == NULL) {
            return false;
        }
        // Insert the new word and its counter into the hashtable
        if (!hashtable_insert(index->hashtable, word, counter)) {
            counters_delete(counter);
            return false;
        }
    }

    // Increment the count for this docID
    counters_add(counter, docID);
    return true;
}

/**************** index_find ****************/
/* see index.h header for more details */
void*
index_find(index_t* index, const char* word)
{
    if (index == NULL || word == NULL) {
        return NULL;
    }
    return hashtable_find(index->hashtable, word);
}

/**************** index_save ****************/
/* see index.h header for more details */
bool
index_save(index_t* index, FILE* fp)
{
    // handle bad parameters
    if (index == NULL || fp == NULL) {
        return false;
    }
    // iterate through hashtable with helper 
    hashtable_iterate(index->hashtable, fp, index_save_helper);
    return true;
}

/**************** index_load ****************/
/* see index.h header for more details */
bool 
index_load(index_t* index, FILE* fp)
{
    if (fp == NULL || index == NULL) {
        return false;
    }

    char word[256];
    int docID, count;

    // read each word from file
    while (fscanf(fp, "%255s", word) == 1) {
        normalizeWord(word);

        // retrieve or create the counter for this word
        counters_t* counter = hashtable_find(index->hashtable, word);
        if (counter == NULL) {
            counter = counters_new();
            // error making counter
            if (counter == NULL) {
                return false;
            }
            // insert the new word and its counter into the hashtable
            if (!hashtable_insert(index->hashtable, word, counter)) {
                counters_delete(counter);
                return false;
            }
        }

        // read all docID and count pairs for this word
        while (fscanf(fp, "%d %d", &docID, &count) == 2) {
            counters_set(counter, docID, count);
        }
    }
    return true;
}

/**************** index_delete ****************/
/* see index.h header for more details */
void 
index_delete(index_t* index)
{
    if (index == NULL) {
        return;
    }

    // delete hashtable
    hashtable_delete(index->hashtable, index_delete_helper);

    // free the index
    free(index);
}