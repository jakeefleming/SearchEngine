/*
 * pagedir.h - Header file for pagedir.c
 *
 * Functions for initializing a pageDirectory
 * and saving pages to it.
 * 
 * Jake Fleming, 10/21/2024
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdbool.h>
#include "../libcs50/webpage.h"

/************** pagedir_init ****************/
/* Initializes page directory to store pages
 * crawled by the crawler. Creates a .crawler
 * file to signify that the directory is being
 * used by the crawler.
 *
 * Inputs:
 *   pageDirectory - Where pages will be saved
 * Return:
 *   True if successful, false otherwise
 */
bool pagedir_init(const char* pageDirectory);

/************** pagedir_save ****************/
/* Saves the given webpage to the pageDirectory
 * with a unique DocumentID, writing the URL,
 * depth, and HTML content
 *
 * Inputs:
 *   page - The webpage to save
 *   pageDirectory - Where to save the webpage
 *   docID - A unique identifier for this page
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/************** pagedir_validate ****************/
/* validate that the given directory is a valid crawler directory
 *
 * Input:
 *   pageDirectory - the path to the directory
 *
 * Return:
 *   true if the directory is valid
 *   false otherwise.
 */
bool pagedir_validate(const char* pageDirectory);

/**************** pagedir_load ****************/
/* Load a webpage from the pageDirectory using the given docID
 * 
 * Inputs:
 *   pageDirectory - the path to the directory
 *   docID - the unique ID of the document
 * 
 * Returns:
 *   A webpage_t object with the URL, depth, and HTML content of the page,
 *   or NULL if the file cannot be loaded
 */
webpage_t* pagedir_load(const char* pageDirectory, int docID);

/**************** getDocumentURL ****************/
/* Get the url of a given document
 * 
 * Inputs:
 *   pageDirectory - the path to the directory
 *   docID - the unique ID of the document
 * 
 * Returns:
 *   A string of the url
 */
char* getDocumentURL(const char* pageDirectory, int docID);


#endif // __PAGEDIR_H