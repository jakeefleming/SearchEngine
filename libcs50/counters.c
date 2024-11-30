
/*
 * counters.c - CS50 'counters' module
 *
 * see counters.h for more information
 * 
 * Jake Fleming, 10/11/24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"

/**************** local types ****************/
typedef struct counternode {
    int key;
    int count;
    struct counternode* next;
} counternode_t;

/**************** global types ****************/
typedef struct counters {
    struct counternode* head;
} counters_t;

/**************** local functions ****************/
/* not visible outside this file */
static counternode_t* counternode_new(int key);

/**************** global functions ****************/
/* that is, visible outside this file */
/* see counters.h for comments about exported functions */

/**************** counters_new ****************/
/* see counters.h for description */
counters_t*
counters_new(void)
{
    counters_t* counters = malloc(sizeof(counters_t));

    if (counters == NULL) {
        return NULL;    // error allocating counters
    }
    else {
        // initialize contents of counter structure
        counters->head = NULL;
        return counters;
    }
}

/**************** counters_add ****************/
/* see counters.h for description */
int
counters_add(counters_t* ctrs, const int key)
{
    if (ctrs != NULL && key >= 0) {
        // search for key in list
        for (counternode_t* node = ctrs->head; node != NULL; 
            node = node->next) {
            if (node->key == key) {
                node->count++;          // increment count
                return node->count;     // return new count
            }
        }
        // if key isn't found, create a new node
        counternode_t* new = counternode_new(key);

        // handle error allocating
        if (new == NULL) {
            return 0;
        }
        // insert new at the head of the list
        new->next = ctrs->head;
        ctrs->head = new;
        return new->count;
    }
    return 0;
}

/**************** counternode_new ****************/
/* Allocate and initialize a counternode */
static counternode_t* 
counternode_new(int key)
{
    counternode_t* node = malloc(sizeof(counternode_t));

    // handle error allocating
    if (node == NULL) {
        return NULL;
    }
    // create new node
    node->key = key;
    node->count = 1;
    node->next = NULL;
    return node;
}

/**************** counters_get ****************/
/* see counters.h for description */
int
counters_get(counters_t* ctrs, const int key)
{
    if (ctrs != NULL && key >= 0) {
        // search for key in list
        for (counternode_t* node = ctrs->head; node != NULL; 
            node = node->next) {
            // return count of key if found
            if (node->key == key) {
                return node->count;
            }
        }
    }
    return 0;
}

/**************** counters_set ****************/
/* see counters.h for description */
bool
counters_set(counters_t* ctrs, const int key, const int count)
{
    if (ctrs != NULL && key >= 0 && count >=0) {
        // search for key in list
        for (counternode_t* node = ctrs->head; node != NULL; 
            node = node->next) {
            // update count if key is found
            if (node->key == key) {
                node->count = count;
                return true;
            }
        }
        // if key isn't found, add it
        counternode_t* new_node = counternode_new(key);
        // handle error allocating
        if (new_node != NULL) {
            // set the count
            new_node->count = count;
            
            // insert the new node at the head of the list
            new_node->next = ctrs->head;
            ctrs->head = new_node;
            return true;
        }
    }
    return false;
}


/**************** counters_print ****************/
/* see counters.h for description */
void
counters_print(counters_t* ctrs, FILE* fp)
{
    if (fp != NULL) {
        if (ctrs != NULL) {
            fputc('{', fp);
            for (counternode_t* node = ctrs->head; node != NULL; 
                node = node->next) {
                // print key-count pair
                fprintf(fp, "%d=%d", node->key, node->count);
                // print ',' if not at the end
                if (node->next != NULL) {
                    fputc(',', fp);
                }
            }
            fputc('}', fp);
            fputs("\n", fp);
        }
        else {
            fputs("(null)\n", fp);
        }
    }
}

/**************** counters_iterate ****************/
/* see counters.h for description */
void
counters_iterate(counters_t* ctrs, void* arg, 
    void (*itemfunc)(void* arg, const int key, const int count))
{
    if (ctrs != NULL && itemfunc != NULL) {
        // call itemfunc with arg on each item
        for (counternode_t* node = ctrs->head; node != NULL;
            node = node->next) {
            (*itemfunc)(arg, node->key, node->count);
        }
    }
}

/**************** counters_delete ****************/
/* see counters.h for description */
void
counters_delete(counters_t* ctrs)
{
    if (ctrs != NULL) {
        for (counternode_t* node = ctrs->head; node != NULL;) {
            counternode_t* next = node->next;
            free(node);
            node = next;
        }
        free(ctrs);
    }
}

/**************** size_helper ****************/
/* increment a counter size */
static void 
size_helper(void* arg, const int key, int count) 
{
    // check it is in the counterset
    if (count > 0) {
        int* size = (int*)arg;
        (*size)++;
    }
}

/**************** counters_size ****************/
/* see counters.h for description */
int
counters_size(counters_t* ctrs)
{
    // invalid pointer
    if (ctrs == NULL) {
        return 0;
    }
    
    // iterate through every key
    int size = 0;
    counters_iterate(ctrs, &size, size_helper);
    return size;
}

/**************** counters_copy ****************/
/* see counters.h for description */
void 
counters_copy(void* arg, const int key, const int count)
{
    // directly set each key-count pair in the destination
    counters_t* dest = arg;
    counters_set(dest, key, count);  
}