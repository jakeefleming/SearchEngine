/*
 * indexer.c - Tiny Search Engine indexer
 * 
 * Reads the documents from the given pageDirectory and builds an index
 * of words and their occurrences across documents. Saves this index
 * to a file
 * 
 * Jake Fleming - 10/28/24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/index.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../libcs50/webpage.h"

/************ Function Prototypes ************/
static index_t* indexBuild(char* pageDirectory);
static void indexPage(index_t* index, webpage_t* page, const int docID);

/***************** main *****************/
/* processes the arguments and runs indexer */
int
main(int argc, char* argv[])
{
    // check for 3 arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: ./indexer pageDirectory indexFilename\n");
        return 1;
    }
    
    // pageDirectory checks
    char* pageDirectory = argv[1];
    if (!pagedir_validate(pageDirectory)) {
        fprintf(stderr, "Error: Invalid pageDirectory: %s\n", pageDirectory);
        return 2;
    }

    // indexFilename checks
    char* indexFilename = argv[2];
    FILE* indexFile = fopen(indexFilename, "w");
    if (indexFile == NULL) {
        fprintf(stderr, "Error: Could not open %s for writing\n", indexFilename);
        return 3;
    }

    index_t* index = indexBuild(pageDirectory);

    // save index to indexFilename
    if (!index_save(index, indexFile)) {
        fprintf(stderr, "Error: Failed to save index to file\n");
        index_delete(index);
        fclose(indexFile);
        return 4;
    }

    // cleanup
    index_delete(index);
    fclose(indexFile);

    return 0;
}

/************ indexBuild ************/
/* Build the index by reading each page
 * in the pageDirectory
 *
 * inputs:
 *   pageDirectory - pointer to pageDirectory
 * 
 * return:
 *   index object
 */
static index_t*
indexBuild(char* pageDirectory)
{
    int docID = 1;
    index_t* index = index_new(500);

    webpage_t* page;
    while ((page = pagedir_load(pageDirectory, docID)) != NULL) {
        // process each webpage and add words to index
        indexPage(index, page, docID);
        docID++;
        webpage_delete(page);
    }
    return index;
}

/************ indexPage ************/
/* Process the given webpage, scan its words, 
 * and add them to the index 
 *
 * inputs:
 *   index - pointer to the index
 *   page - pointer to the page we save
 *   docID - id of current document
 */
static void
indexPage(index_t* index, webpage_t* page, const int docID)
{
    int pos = 0;
    char* word;

    // extract word from webpage
    while ((word = webpage_getNextWord(page, &pos)) != NULL) {
        // ignore words with fewer than 3 characters
        if (strlen(word) >= 3) {
            normalizeWord(word);
            // add or increment word count in index
            index_insert(index, word, docID);
        }
        free(word);
    }
}