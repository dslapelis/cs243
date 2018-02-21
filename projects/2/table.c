#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "table.h"
#include "hash.h"

/// custom hash function
void rehash(Table* t, Entry** new_table, Entry* ent);

/// implemented functions begin

Table* create(long (*hash)(void* key),
              bool (*equals)(void* key1, void* key2),
              void (*print)(void* key1, void* key2))
{
    /// create a table
    Table* t;
    t = malloc(sizeof(Table));
    if (t == NULL) {
        fprintf(stderr, "table::create() failed to allocate space");
        assert(NULL);
    }

    Entry** table;
    
    /// initializing a null entry table
    table = calloc(INITIAL_CAPACITY, sizeof(char*));
    for(size_t x = 0; x < INITIAL_CAPACITY; x++)
    {   
        table[x] = NULL;
    }   

    t->table = table;
    
    /// here we point the pointers to the functions defined in hash.h
    t->hash = hash;
    t->equals = equals;
    t->print = print;

    /// assigning struct members some values
    t->size = 0;
    t->capacity = INITIAL_CAPACITY;

    t->collisions = 0;
    t->rehashes = 0;

    return t;
}

void destroy(Table* t)
{
    for(size_t x = 0; x < t->capacity; x++)
    {
        free(t->table[x]);
    }
    free(t->table);
    free(t);
}

void dump(Table* t, bool full)
{
    printf("Size: %zd\n", t->size);
    printf("Capacity: %zd\n", t->capacity);
    printf("Collisions: %zd\n", t->collisions);
    printf("Rehashes: %zd\n", t->rehashes);
    if(full)
    {
        for( size_t x = 0; x < t->capacity; x++)
        {
            if(t->table[x] != NULL)
            {
                 printf("%zd: (", x);
                 t->print(t->table[x]->key, t->table[x]->value);
                 printf(")\n");
            }
            else
            {
                printf("%zd:", x);
                printf("null\n");
            }
         }
     }
}

void* get(Table* t, void* key)
{
    /// position in array to start the search at 
    size_t hashIndex = t->hash(key);
    //printf("%zd || %zd\n", hashIndex, t->capacity);
    if(hashIndex > t->capacity)
    {
        hashIndex %= t->capacity;
    }
    
    /// if our index is larger than the hashtable, we start our search at 0.
    while( t->table[hashIndex] != NULL )
    {
        if( t->equals(t->table[hashIndex]->key, key ))
        {
            //printf("returning a song value\n");
            /// return the song
            return t->table[hashIndex]->value;
        }
        else
        {
            t->collisions += 1;
            hashIndex++;

            /// loopback
            hashIndex %= t->capacity;
        }
    }
    
    /// value is not in the table, so we return null
    printf("returning null\n");
    return NULL;
}

bool has(Table* t, void* key)
{
    /// position in array to start the search at 
    size_t hashIndex = t->hash(key);
    /// if our index is larger than the capacity we start at 0
    if(hashIndex > t->capacity)
    {
        hashIndex %= t->capacity;
    }
    
    //size_t x = 1;
    /// searching for the key
    while( t->table[hashIndex] != NULL )
    {
        //printf("%zd: LOOPED\n", x);
        //x++;
        
        if( t->equals(t->table[hashIndex]->key, key ))
        {
            /// return the song
            return true;
        }
        else
        {
            t->collisions += 1;
            hashIndex++;
            
            /// loopback when at end of capacity
            hashIndex %= t->capacity;
        }
        
    }
    
    /// if we hit a null index, the table does not have this key
    return false;
}

void** keys(Table* t)
{
    void** keys;
    //printf("SIZE: %zd\n", t->size);
    keys = malloc(t->size * sizeof(long));

    /// index variable so we can add keys to array
    /// without going over the size
    size_t index = 0;
    for(size_t x = 0; x < t->capacity; x++)
    {
        //printf("ELEMENT %zd\n", x);
        if(t->table[x] != NULL)
        {
            keys[index] = t->table[x]->key;
            index++;
        }
    }
    return keys;
}


void* put(Table* t, void* key, void* value)
{
    /// if load threshhold is >= 0.75 rehash
    double current_load = (double)t->size / (double)t->capacity;
    //printf("SIZE: %zd, CAPACITY: %zd LOAD: %lf\n", t->size, t->capacity, current_load);
    if( current_load >= LOAD_THRESHOLD )
    {
       // char* ptr = &t;
        //printf("%s\n", ptr);
        /// -------BEGIN REHASHING-------
        /// creates a new table
        /// initializes its members and allocates a new table
        /// of twice the capacity
        size_t new_cap = t->capacity * 2;
        Entry** table;
        table = calloc(t->capacity * 2, sizeof(char*));
        for(size_t x = 0; x < t->capacity * 2; x++)
        {
            table[x] = NULL;
        }

        /// copy all non-null entries to the new table
        for(size_t x = 0; x < t->capacity; x++)
        {
            if(t->table[x] != NULL)
            {
                //printf("REHASHING \n");
                rehash(t, table, t->table[x]);
                free(t->table[x]);
            }
        }

        free(t->table);

        /// point t->table to our new table
         t->table = table;
         t->rehashes += 1;
         t->capacity = new_cap;
        /// -------END REHASHING-------
     }
    /// if load threshold is OK then we just try and add to the hash
        size_t hashIndex = t->hash(key);
        
        if(hashIndex > t->capacity)
        {
            hashIndex %= t->capacity;
        }
        
        size_t start = hashIndex;

        /// cycling through the array until we reach a null value.
        while(t->table[hashIndex] != NULL)
        {
            /// if we reach a non-null value and the keys are equal, we
            /// update the value and return the old value
            if(t->equals(t->table[hashIndex]->key, key))
            {
                void* old_song = t->table[hashIndex]->value;
                t->table[hashIndex]->value = value;
                return old_song;
            }
            else
            {
                t->collisions += 1;
                hashIndex++;
                if( hashIndex == start )
                {
                    return NULL;
                }
                hashIndex %= t->capacity;
            }
        }
        /// if we reach a null position, we can add a new entry
        /// ---DEBUGGING---
        //printf("HASH INDEX: %zd\n", hashIndex);
        //printf("TABLE CAPACITY: %zd\n", t->capacity);
        Entry* ent = malloc(sizeof(Entry));
        t->table[hashIndex] = ent;
        t->table[hashIndex]->key = key;
        t->table[hashIndex]->value = value;
        t->size += 1;
        return NULL;
}


void** values(Table* t)
{
void** values;
    
     values = malloc(t->size * sizeof(char*));
    
    size_t index = 0;
    for(size_t x = 0; x < t->capacity; x++)
    {
        //printf("ELEMENT %zd\n", x);
        if(t->table[x] != NULL)
        {
            values[index] = t->table[x]->value;
            index++;
        }
    }
    return values;
}


void rehash(Table* t, Entry** new_table, Entry* ent)
{
    size_t hashIndex = t->hash(ent->key);

    /// if index is out of bounds, make in in bounds of hashtable
    if(hashIndex > t->capacity * 2)
    {
        hashIndex %= t->capacity * 2;
    }

    while(new_table[hashIndex] != NULL)
    {
        hashIndex++;
        t->collisions++;
        hashIndex %= t->capacity * 2;
    }
    /// when we get to a null position we can add the entry here
    Entry* new_ent = malloc(sizeof(Entry));
    new_table[hashIndex] = new_ent;
    new_table[hashIndex]->key = ent->key;
    new_table[hashIndex]->value = ent->value;
    //free(ent);
}
