// word.h - header file for word module
//
// Provides utilities for processing words in the indexer
//
// Jake Fleming, 10/28/2024

#ifndef __WORD_H
#define __WORD_H

#include <stdbool.h>

/**************** normalizeWord ****************/
/* Normalize the given word to lowercase. 
 * Only modifies alphabetic characters (A-Z, a-z).
 * The function modifies the word in place.
 *
 * Inputs:
 *   word - a pointer to the word to normalize
 */
void normalizeWord(char* word);

#endif // __WORD_H