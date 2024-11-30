/*
 * hashtable.c - CS50 'hashtable' module
 *
 * see hashtable.h for more information
 * 
 * Jake Fleming, 10/11/24
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "hash.h"
#include "set.h"


/**************** global types ****************/
typedef struct hashtable {
    int num_slots;
    set_t** slots;
} hashtable_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** hashtable_new ****************/
/* see hashtable.h for description */
hashtable_t*
hashtable_new(const int num_slots)
{
    if (num_slots > 0) {
        hashtable_t* hashtable = malloc(sizeof(hashtable_t));
        
        // handle error allocating
        if (hashtable == NULL) {
            return NULL;
        }
        // allocate memory for the array of set pointers
        hashtable->slots = malloc(num_slots * sizeof(set_t*));

        // handle error allocating
        if (hashtable->slots == NULL) {
            free(hashtable);
            return NULL;
        }
        hashtable->num_slots = num_slots;

        // initialize each slot to an empty set
        for (int i = 0; i < num_slots; i++) {
            hashtable->slots[i] = set_new();

            // handle error allocating
            if (hashtable->slots[i] == NULL) {
                // delete all of the previously created sets
                for (int j = 0; j < i; j++) {
                    set_delete(hashtable->slots[j], NULL);
                }
                // free the memory
                free(hashtable->slots);
                free(hashtable);
                return NULL;
            }
        }
        return hashtable;
    }
    return NULL;
}

/**************** hashtable_insert ****************/
/* see hashtable.h for description */
bool
hashtable_insert(hashtable_t* ht, const char* key, void* item)
{
    if (ht != NULL && key != NULL && item != NULL) {
        // hash the key to determine the slot
        unsigned long hash = hash_jenkins(key, ht->num_slots);

        // insert key-item pair into appropriate slot
        // set_insert handles the key copying 
        return set_insert(ht->slots[hash], key, item);
    }
    return false;
}

/**************** hashtable_find ****************/
/* see hashtable.h for description */
void*
hashtable_find(hashtable_t* ht, const char* key)
{
    if (ht != NULL && key != NULL) {
        // hash the key to find the slot
        unsigned long hash = hash_jenkins(key, ht->num_slots);

        // find the item in the appropriate slot
        return set_find(ht->slots[hash], key);
    }
    return NULL;
}

/**************** hashtable_print ****************/
/* see hashtable.h for description */
void
hashtable_print(hashtable_t* ht, FILE* fp, 
    void (*itemprint)(FILE* fp, const char* key, void* item))
{
    if (fp != NULL) {
        if (ht != NULL) {
            // loops over slots
            for (int i = 0; i < ht->num_slots; i++) {
                // print each set
                if (itemprint != NULL) {
                    fprintf(fp, "Slot %d: ", i);
                    set_print(ht->slots[i], fp, itemprint);
                }
                
            }
        }
        else {
            fputs("(null)\n", fp);
        }
    }
}

/**************** hashtable_iterate ****************/
/* see hashtable.h for description */
void
hashtable_iterate(hashtable_t* ht, void* arg,
    void (*itemfunc)(void* arg, const char* key, void* item) )
{
    if (ht != NULL && itemfunc != NULL) {
        // iterate over each set
        for (int i = 0; i < ht->num_slots; i++) {
            set_iterate(ht->slots[i], arg, itemfunc);
        }
    }
}

/**************** hashtable_delete ****************/
/* see hashtable.h for description */
void
hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item) )
{
    if (ht != NULL) {
        // delete each set
        for (int i = 0; i < ht->num_slots; i++) {
            if (itemdelete != NULL) {
                set_delete(ht->slots[i], itemdelete);
            }
        }
        // free slots and hashtable
        free(ht->slots);
        free(ht);
    }
}