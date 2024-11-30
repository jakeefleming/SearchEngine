
/*
 * crawler.c - Tiny Search Engine crawler module
 *
 * Perform the recursive crawl on every page we
 * visit up through a certain depth of internalURLs
 * and save the pages to pageDirectory
 * 
 * Jake Fleming, 10/21/24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"


// function prototypes
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
void dummyDelete(void* item);

/***************** main ******************/
/* Call parseArgs and crawl, then exit 0 
 *
 * Inputs:
 *   seedURL - URL of where crawler starts
 *   pageDirectory - where crawled pages are saved
 *   maxDepth - limits how far crawler should go
 */
int
main(int argc, char* argv[])
{
    char* seedURL = malloc(strlen(argv[1]) + 1);
    char* pageDirectory;
    int maxDepth;

    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);

    return 0;
}

/***************** parseArgs ******************/
/* Check all the inputs are corrects and parse.
 * SeedURL must be normal and internal
 * pageDirectory must be properly initialized
 * maxDepth must be between 0 and 10
 *
 * Inputs:
 *   seedURL - URL of where crawler starts
 *   pageDirectory - where crawled pages are saved
 *   maxDepth - limits how far crawler should go
 */
static void
parseArgs(const int argc, char* argv[],
            char** seedURL, char** pageDirectory, int* maxDepth)
{
    // ensure 3 arguments were passed
    if (argc != 4) {
        fprintf(stderr, "Usage: ./crawler seedURL pageDirectory maxDepth\n");
        exit(1);
    }

    // seedURL checks
    strcpy(*seedURL, argv[1]);
    char* normalizedSeedURL = normalizeURL(*seedURL);

    // ensure normal URL 
    if (!normalizedSeedURL) {
        fprintf(stderr, "Error: Invalid seedURL\n");
        exit(2);
    }
    // ensure internal URL
    if (!isInternalURL(*seedURL)) {
        fprintf(stderr, "Error: Seed URL is not internal\n");
        exit(3);
    }

    // pageDirectory checks
    *pageDirectory = argv[2];

    // initialize pageDirectory
    if (!pagedir_init(*pageDirectory)) {
        fprintf(stderr, "Error: Could not initialize page directory\n");
        exit(4);
    }

    // max depth checks
    *maxDepth = atoi(argv[3]);

    // ensure between 0 and 10
    if (*maxDepth < 0 || *maxDepth > 10) {
        fprintf(stderr, "Error: Max depth must be between 0 and 10\n");
        exit(5);
    }
    free(*seedURL);
    *seedURL = normalizedSeedURL;
}

/***************** crawl ******************/
/* Perform the web crawl on every webpage
 * in the bag, checking if they have been
 * visited and scanning them into the bag
 * if we haven't reached the maxDepth
 *
 * Inputs: 
 *   seedURL - URL of where crawler starts
 *   pageDirectory - where crawled pages are saved
 *   maxDepth - limits how far crawler should go
 */
static void
crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{
    // initialize data structures and docID
    hashtable_t* pagesSeen = hashtable_new(200);
    bag_t* pagesToCrawl = bag_new();
    int docID = 1;

    // add seedURL to both
    hashtable_insert(pagesSeen, seedURL, "");
    bag_insert(pagesToCrawl, webpage_new(seedURL, 0, NULL));

    // loop through every URL in the bag
    webpage_t* page;
    while ((page = bag_extract(pagesToCrawl)) != NULL) {
        int depth = webpage_getDepth(page);

        // fetch html - sleep(1) is within this method
        if (webpage_fetch(page)) {
            // log
            printf("%d    Fetched: %s\n", depth, webpage_getURL(page));

            // save webpage to pageDirectory
            pagedir_save(page, pageDirectory, docID);
            docID++;
            
            // scan html if we haven't reached maxDepth
            if (webpage_getDepth(page) < maxDepth) {
                // log
                printf("%d   Scanning: %s\n", depth, webpage_getURL(page));
                pageScan(page, pagesToCrawl, pagesSeen);
            }
        }
        // log
        else {
            fprintf(stderr, "Failed to fetch %s\n", webpage_getURL(page));
        }
        // free memory and sleep between fetches
        webpage_delete(page);
    }
    // free memory
    hashtable_delete(pagesSeen, dummyDelete);
    bag_delete(pagesToCrawl, webpage_delete);
}

/********************* pageScan **********************/
/* Scan all of the internal URLs within a page and
 * add them to pagesToCrawl so the crawler can
 * continue to run
 *
 * Inputs:
 *   page - the page to scan
 *   pagesToCrawl - the bag of pages we need to visit
 *   pagesSeen - the hashtable of pages we visited
 */
static void
pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
    int pos = 0;
    char* nextURL;

    // loop while there are more URLs in the page
    while ((nextURL = webpage_getNextURL(page, &pos)) != NULL) {
        // normalize the URL
        char* normalizedURL = normalizeURL(nextURL);
        
        // log
        printf("%d      Found: %s\n", webpage_getDepth(page), nextURL);
        free(nextURL);

        if (isInternalURL(normalizedURL)) {
            // check if it is in the hashtable aleady and insert if not
            if (hashtable_insert(pagesSeen, normalizedURL, "")) {
                // log
                printf("%d      Added: %s\n", webpage_getDepth(page), normalizedURL);

                // add to pagesToCrawl at a higher depth
                webpage_t* newPage = webpage_new(normalizedURL, webpage_getDepth(page) + 1, NULL);
                bag_insert(pagesToCrawl, newPage);
            }
            else {
                // log
                printf("%d    IgnDupl: %s\n", webpage_getDepth(page), normalizedURL);

                // free memory
                free(normalizedURL);
            }
        }
        else {
            // log
            printf("%d   IgnExtrn: %s\n", webpage_getDepth(page), normalizedURL);

            // free memory if it wasn't an internalURL
            free(normalizedURL);
        }
    }
}

/********************* dummyDelete **********************/
/* Function that does not hing so taht we can pass a
 * non-null itemfunc into hashtable_delete so that all 
 * of the sets get freed.
 */
void dummyDelete(void* item) {return;}