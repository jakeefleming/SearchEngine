// index.h - header file for the index module
// 
// Provides the interface for an index, a data structure
// that maps words to (documentID, count) pairs
//
// Jake Fleming, 10/28/2024

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdbool.h>

/**************** index_t ****************/
// Opaque structure that represents the index
typedef struct index index_t;

/**************** index_new ****************/
/* 
 * Create a new index. Caller is responsible for
 * later freeing this.
 *
 * Inputs:
 *   num_slots - the initial size of hashtable
 * 
 * Return:
 *   a pointer to the new index, or NULL if error
 */
index_t* index_new(const int num_slots);

/**************** index_insert ****************/
/* 
 * Insert a word with the given document ID and count into the index
 *
 * Inputs:
 *   index - a pointer to the index
 *   word - a string representing the word
 *   docID - the document ID
 *
 * Return:
 *   true if insertion was successful, false otherwise
 */
bool index_insert(index_t* index, const char* word, int docID);

/**************** index_find ****************/
/*
 * Find the set of (docID, count) pairs for 
 * the given word in the index
 * 
 * Inputs:
 *   index - a pointer to the index
 *   word - a string representing the word
 * 
 * Return:
 *   a pointer to a data structure that holds the docID and counts,
 *   or NULL if the word is not found
 */
void* index_find(index_t* index, const char* word);

/**************** index_save ****************/
/*
 * Save the index to a file
 * Each line of the output will be formatted as follows:
 *   word docID count [docID count]...
 * 
 * Inputs:
 *   index - a pointer to the index
 *   fp - a writable FILE* for the output file
 * 
 * Return:
 *   true if successful, false otherwise
 */
bool index_save(index_t* index, FILE* fp);

/**************** index_load ****************/
/*
 * Load an index from a file.
 * 
 * Inputs:
 *   index - a pointer to an empty index
 *   fp - a readable FILE* for the input file
 * 
 * Returns:
 *   true if successful, false otherwise
 */
bool index_load(index_t* index, FILE* fp);

/**************** index_delete ****************/
/*
 * Delete the entire index, freeing all memory
 * 
 * Caller provides:
 *   index - a pointer to the index
 */
void index_delete(index_t* index);

#endif // __INDEX_H