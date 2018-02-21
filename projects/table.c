#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "table.h"
#include "hash.h"

/// implemented functions begin

Table* create(long (*hash)(void* key),
              bool (*equals)(void* key1, void* key2),
	      void (*print)(void* key1, void* key2))
{
    /// create a table
    Table* t;
    t = malloc(sizeof(Table));

    Entry** table;
    
    /// initializing a null entry table
    table = calloc(INITIAL_CAPACITY, sizeof(char*));
    for(size_t x = 0; x < INITIAL_CAPACITY; x++)
    {
        table[x] = NULL;
    }
    
    /// here we point the pointers to the functions defined in hash.h
    hash = longHash;
    equals = longEquals;
    print = longStrPrint;

    /// assigning struct members some values
    size_t size = 0;
    size_t capacity = INITIAL_CAPACITY;

    size_t collisions = 0;
    size_t rehashes = 0;

    return t;
}

void destroy(Table* t)
{
    for(size_t x = 0; x < t->capacity; x++)
    {
        free(t->table[x]);
    }
    free(t->table);

    //free(t->size);
    //free(t->capacity);
    //free(t->key);
    //free(t->are_equal);
    //free(t->collisions);
    //free(t->rehashes);
}

void dump(Table* t, bool full)
{
    printf("-----HASH TABLE INFORMATION-----\n");
    printf("ENTRY_TABLE SIZE: %zd\n", t->size);
    printf("ENTRY_TABLE CAPACITY: %zd\n", t->capacity);
    printf("HASH TABLE REHASHES: %zd\n", t->rehashes);
    printf("HASH TABLE COLLISIONS: %zd\n", t->collisions);
    
    if(full)
    {
        for(size_t x = 0; x < t->capacity; x++)
        {
            printf("**ENTRY_TABLE[%zd]**\n", x);
            printf("Key: %p\n", t->table[x]->key);
            printf("Value: %p\n", t->table[x]->value);
        }
    }
    
    printf("--------------------------------\n");
}

void* get(Table* t, void* key)
{
    Entry* temp;

    /// position in array to start the search at 
    size_t hashIndex = t->hash(key);

    while( t->table[hashIndex] != NULL )
    {
        temp = t->table[hashIndex];
        if( t->equals(temp->key, key ))
        {
            /// return the song
            return temp->value;
        }
        
        hashIndex++;
        hashIndex %= t->size;
    }
        
    return NULL;
}

bool has(Table* t, void* key)
{
   Entry* temp;
 
   // position in array to start the search at 
    size_t hashIndex = t->hash(key);

    while( t->table[hashIndex] != NULL )
    {
        temp = t->table[hashIndex];
        if( t->equals(temp->key, key ))
        {
            /// return the song
            return true;
        }

        hashIndex++;
        hashIndex %= t->size;
    }

    return false;

}
    
void** keys(Table* t)
{
    void** keys = malloc(t->size);
    for( size_t x = 0; x < t->size; x++ )
    {
        Entry* temp = t->table[x];
        keys[x] = temp->key;
    }
    return keys;
}

void* put(Table* t, void* key, void* value)
{
    long hashIndex = t->hash(key);
    Entry* temp;

    /// search the table for a key
    /// if key exists in the position, check to see if 
    /// keys are equal. if not, try next position until
    /// they are equal. If key at position is null, insert
    /// value and key in that position.
    while( t->table[hashIndex] != NULL )
    {
        if( t->equals(temp->key, key) )
        {
            temp = t->table[hashIndex];
            void* value = temp->value;
            temp->value = value;
            return value;
        }
        hashIndex++;
        hashIndex %= t->size;
     }
     /// if position is NULL and not equal to the key
     temp = t->table[hashIndex];
     temp->value = value;
     temp->key = key;
     return NULL;
}
