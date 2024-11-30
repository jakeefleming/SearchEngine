/*
 * set.c - CS50 'set' module
 *
 * see set.h for more information
 * 
 * Jake Fleming, 10/11/24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"


/**************** local types ****************/
typedef struct setnode {
    char* key;
    void* item;
    struct setnode* next;
} setnode_t;

/**************** global types ****************/
typedef struct set {
    struct setnode* head;
} set_t;


/**************** local functions ****************/
/* not visible outside this file */
static setnode_t* setnode_new(char* key, void* item);

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** set_new ****************/
/* see set.h for description */
set_t*
set_new(void)
{
    set_t* set = malloc(sizeof(set_t));

    if (set == NULL) {
        return NULL;    // error allocating set
    }
    else {
        // initialize contents of set structure
        set->head = NULL;
        return set;
    }
}

/**************** set_insert ****************/
/* see set.h for description */
bool
set_insert(set_t* set, const char* key, void* item)
{
    if (set != NULL && key != NULL && item != NULL) {
        // check if key is already in set
        if (set_find(set, key) != NULL) {
            return false;
        }
        // allocate memory for key copy
        char* key_copy = malloc(strlen(key) + 1);

        // handle error allocating
        if (key_copy == NULL) {
            return false;
        }
        strcpy(key_copy, key);

        // create new setnode
        setnode_t* new = setnode_new(key_copy, item);

        // hanlde error allocating
        if (new == NULL) {
            free(key_copy);
            return false;
        }
        // insert new at the head of the list
        new->next = set->head;
        set->head = new;
        return true;
    }
    return false;
}

/**************** setnode_new ****************/
/* Allocate and initialize a setnode */
static setnode_t*
setnode_new(char* key, void* item)
{
    setnode_t* node = malloc(sizeof(setnode_t));

    // handle error allocating
    if (node == NULL) {
        return NULL;
    }
    // create new node
    node->key = key;
    node->item = item;
    node->next = NULL;
    return node;
}

/**************** set_find() ****************/
/* see set.h for description */
void*
set_find(set_t* set, const char* key)
{
    if (set != NULL && key != NULL) {
        for (setnode_t* node = set->head; node != NULL; 
            node = node->next) {
            // compare strings
            if (strcmp(node->key, key) == 0) {
                return node->item;
            }
        }
    }
    return NULL;
}

/**************** set_print ****************/
/* see set.h for description */
void
set_print(set_t* set, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item) )
{
    if (fp != NULL) {
        if (set != NULL) {
            fputc('{', fp);
            for (setnode_t* node = set->head; node != NULL; 
                node = node->next) {
                // print this node
                if (itemprint != NULL) {
                    (*itemprint)(fp, node->key, node->item);
                    // print commas between nodes
                    if (node->next != NULL) {
                        fputc(',', fp);
                    }
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

/**************** set_iterate ****************/
/* see set.h for description */
void
set_iterate(set_t* set, void* arg, void (*itemfunc)(void* arg, const char* key, void* item) ) 
{
    if (set != NULL && itemfunc != NULL) {
        // call itemfunc with arg on each item
        for (setnode_t* node = set->head; node != NULL; 
            node = node->next) {
            (*itemfunc)(arg, node->key, node->item);
        }
    }
}

/**************** set_delete ****************/
/* see set.h for description */
void
set_delete(set_t* set, void (*itemdelete)(void* item) ) 
{
    if (set != NULL) {
        for (setnode_t* node = set->head; node != NULL;) {
            //
            if (itemdelete != NULL) {
                (*itemdelete)(node->item);
            }
            setnode_t* next = node->next;   // remember what comes next
            free(node->key);                // free the key
            free(node);                     // free the node
            node = next;                    // and move on to next
        }
        free(set);
    }
}