/*
 * indextest.c - Test program for the Tiny Search Engine indexer
 * 
 * Loads an index from an existing file and saves it to a new file,
 * allowing verification that loading and saving preserve data integrity
 * 
 * Jake Fleming - 10/28/24
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../common/index.h"

/************ main *************/
/* test index_load and index_save
 * by loading an old index file 
 * into a new one
 * 
 * Usage:
 *   ./indextest oldIndexFile newIndexFile
 */
int 
main(int argc, char* argv[])
{
    // ensure correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: ./indextest oldIndexFilename newIndexFilename\n");
        return 1;
    }

    // open the old index file for reading
    char* oldIndexFilename = argv[1];
    FILE* oldFile = fopen(oldIndexFilename, "r");
    if (oldFile == NULL) {
        fprintf(stderr, "Error: Could not open %s for reading\n", oldIndexFilename);
        return 2;
    }

    // create a new index structure and load data from the file
    index_t* index = index_new(500);
    if (index == NULL) {
        fprintf(stderr, "Error: Could not create index structure\n");
        fclose(oldFile);
        return 3;
    }

    // load the old index file into the index structure
    if (!index_load(index, oldFile)) {
        fprintf(stderr, "Error: Failed to load index from file %s\n", oldIndexFilename);
        index_delete(index);
        fclose(oldFile);
        return 4;
    }

    fclose(oldFile);

    // open the new index file for writing
    char* newIndexFilename = argv[2];
    FILE* newFile = fopen(newIndexFilename, "w");
    if (newFile == NULL) {
        fprintf(stderr, "Error: Could not open %s for writing\n", newIndexFilename);
        index_delete(index);
        return 5;
    }

    // save the index structure to the new file
    if (!index_save(index, newFile)) {
        fprintf(stderr, "Error: Failed to save index to file %s\n", newIndexFilename);
        index_delete(index);
        fclose(newFile);
        return 6;
    }

    // cleanup
    index_delete(index);
    fclose(newFile);

    printf("Index successfully loaded from %s and saved to %s\n", oldIndexFilename, newIndexFilename);
    return 0;
}