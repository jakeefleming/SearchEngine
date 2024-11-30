/*
 * pagedir.c - Tiny search engine pagedir module
 *
 * Functions for initializing a pageDirectory
 * and saving pages to it.
 * 
 * Jake Fleming, 10/28/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"

/************* pagedir_init ***************/
/* See pagedir.h for detailed description */
bool
pagedir_init(const char* pageDirectory)
{
    // create the pathname for .crawler file
    // allocate space for "/.crawler\0"
    char* crawlerFilePath = malloc(strlen(pageDirectory) + 10); 
    
    // error allocating
    if (crawlerFilePath == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for crawler file path\n");
        return false;
    }

    // construct pathname in directory
    sprintf(crawlerFilePath, "%s/.crawler", pageDirectory);

    // open the file for writing; on error, return false 
    FILE* file = fopen(crawlerFilePath, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not create .crawler file in directory %s\n", pageDirectory);
        free(crawlerFilePath);
        return false;
    }

    // close the file and return true.
    fclose(file);
    free(crawlerFilePath);
    return true;
}

/************* pagedir_save ***************/
/* See pagedir.h for detailed description */
void
pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{
    // construct the pathname for the page file in pageDirectory
    // room for "/<docID>\0" assuming 6 digits, so crawler should never
    // search more than 999,999 pages
    char* filePath = malloc(strlen(pageDirectory) + 8);

    // error allocating
    if (filePath == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for file path\n");
        exit(6);
    }

    // filePath = "<pageDirectory>/<docID>"
    sprintf(filePath, "%s/%d", pageDirectory, docID);

    // open file for writing
    FILE* file = fopen(filePath, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s for writing\n", filePath);
        free(filePath);
        exit(7);
    }

    // write the URL, depth, and HTML content
    fprintf(file, "%s\n", webpage_getURL(page));
    fprintf(file, "%d\n", webpage_getDepth(page));
    fprintf(file, "%s", webpage_getHTML(page));

    // close the file
    fclose(file);
    free(filePath);
}

/************** pagedir_validate ****************/
/* See pagedir.h for detailed description */
bool
pagedir_validate(const char* pageDirectory)
{
    // check if pageDirectory exists and is a directory
    // help from chatGPT on syntax
    struct stat dirInfo;
    if (stat(pageDirectory, &dirInfo) != 0 || !S_ISDIR(dirInfo.st_mode)) {
        return false;
    }
    // create path to .crawler file
    // make space for ./crawler\0
    char crawlerFile[strlen(pageDirectory) + 10];
    sprintf(crawlerFile, "%s/.crawler", pageDirectory);

    // check if it exists
    // help from chatGPT on syntax
    if (access(crawlerFile, F_OK) != 0) {
        return false;
    }
    return true;
}

/**************** pagedir_load ****************/
/* See pagedir.h for detailed description */
webpage_t*
pagedir_load(const char* pageDirectory, int docID)
{
    // construct the file path for the document
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s/%d", pageDirectory, docID);

    // open the file
    FILE* fp = fopen(filePath, "r");

    // error opening
    if (fp == NULL) {
        return NULL;
    }

    // read the URL
    char* url = file_readLine(fp);
    if (url == NULL) {
        fclose(fp);
        return NULL;
    }

    // read the depth
    int depth;
    if (fscanf(fp, "%d\n", &depth) != 1) {
        free(url);
        fclose(fp);
        return NULL;
    }

    // read the HTML content using file_readFile
    char* html = file_readFile(fp);
    fclose(fp);

    if (html == NULL) {
        free(url);
        return NULL;
    }

    // create webpage object
    webpage_t* page = webpage_new(url, depth, html);
    if (page == NULL) {
        free(url);
        free(html);
        return NULL;
    }
    return page;
}

/**************** getDocumentURL ****************/
/* See pagedir.h for detailed description */
char* 
getDocumentURL(const char* pageDirectory, int docID) 
{
    // construct filePathname
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s/%d", pageDirectory, docID);

    FILE *fp = fopen(filePath, "r");
    if (fp == NULL) return NULL;

    // get url out of file
    char* url = file_readLine(fp);
    fclose(fp);
    return url;
}