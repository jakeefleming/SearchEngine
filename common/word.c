// word.c - implementation file for word module
//
// Provides utilities for processing words in the indexer
//
// Jake Fleming, 10/28/2024

#include <stdlib.h>
#include <ctype.h>
#include "word.h"

/************ normalizeWord ************/
/* see word.h for more details */
void 
normalizeWord(char* word) 
{
    if (word == NULL) {
        return; // handle null pointer
    }

    for (char* c = word; *c != '\0'; c++) {
        if (isalpha(*c)) {
            *c = tolower(*c);
        }
    }
}