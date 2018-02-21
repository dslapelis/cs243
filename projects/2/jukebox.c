/// ---AUTHOR: daniel slapelis---
/// ---ID: djs7209---
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "jukebox.h"
#include "hash.h"

int main(int argc, char* argv[])
{

    /// if not supplied enough arguments, exit failure.
    if(argc != 2)
    {
        fprintf(stderr, "Usage: jukebox filename\n");
        return EXIT_FAILURE;
    }

    
    /// set file up for reading
    /// load a new table with songs from file,
    /// run the simulation, print the statistics, close the table.
    FILE *file = fopen(argv[1], "r");
    if(file != NULL)
    {
        Table* table = loadJukebox(file);
        runSimulation(table);
        printStatistics(table);
        destroy(table);
    }
    else
    {
        fprintf(stderr, "Error reading file %s\n", argv[1]);
    }
    return EXIT_SUCCESS;
}

/// returns hash for an element
long songHash(void* element)
{
    long hash = strHash(element);
    return hash;
}

/// returns the equality of two elements
bool songEquals(void* element1, void* element2)
{
    Song* el1 = element1;
    Song* el2 = element2;

    if(strEquals(el1->artist, el2->artist) && strEquals(el1->song, el2->song))
    {
        return true;
    }
    return false;
}

/// prints the artist, song, and value
void songPrint(void* key, void* value)
{
    Song* tmpsong = key;
    int *playsPtr = (int *)value;
    printf("SONG: %s by %s PLAYS: %d\n", tmpsong->song, tmpsong->artist, *playsPtr);
}

/// for loading the jukebox
Table* loadJukebox(FILE* fp)
{
    /// reading file
    Table* table = create(strHash, strEquals, longStrPrint);
    char string[MAX_LINE_LENGTH];
    while(fgets(string, MAX_LINE_LENGTH, fp) != NULL)
    { 
        /// variables
        int count = 0;
        char* artist;
        char* song;
        char* token = strtok(string, SEP);

            /// while the token isnt null, check the count
            /// depending on the count, print the artist or song.
            /// Create a new song
            while(token != NULL)
            {
                if(count == 4)
                {
                   artist = strdup(token);
                                       
                }
                if(count == 6)
                {
                    token[strlen(token)-1] = '\0';
                    song = strdup(token);
                }
                token = strtok(NULL, SEP);
                count++;
            }
        Song* new_song = malloc(sizeof(Song));
        new_song->artist = artist;
        new_song->song = song;
        long* plays = malloc(sizeof(long));
        *plays = 0;

        /// if the table doesnt have the song, add it
        if(!has(table, new_song))
        {
            put(table, new_song, plays);
            plays = NULL;
            free(plays);
            new_song = NULL;
            free(new_song);
        }
        else
        {
            /// if the table already contains the song,
            /// free the memory we allocated to the song.
            new_song = NULL;
            plays = NULL;
            free(plays);
            free(new_song);
        }
        new_song = NULL;
        free(new_song);
        plays = NULL;
        free(plays);
        
    }
    fclose(fp);
    return table;
}

void printStatistics(Table* jukebox)
{
    /// finding the most and least played songs
    Entry* most_played;
    Entry* least_played;
    //Song* curr = malloc(sizeof(Song));
    size_t x = 0;
    while(jukebox->table[x] == NULL)
    {
        x++;
    }
    
    /// most and least are initialized to first non-null
    most_played = jukebox->table[x];
    least_played = jukebox->table[x];
    Entry* current;
    int numsongs = 0;
    int avgplays = 0;
    int* mostplayedval = (int*)most_played->value;
    int* leastplayedval = (int*)least_played->value;
    
    for(size_t y = 0; y < jukebox->capacity; y++)
    {
        current = jukebox->table[y];
        if(current != NULL)
        {
            int* currval = (int*)current->value;
            if(*currval != 0)
            {
                numsongs += *currval;
            }
            avgplays += *currval;
            if(*mostplayedval < *currval)
            {
                //printf("%zd MP LESS THAN CURR\n", y);
                most_played = current;
                mostplayedval = currval;
            }
  
            if(*leastplayedval > *currval)
            {
                //printf("%zd LP GREATER THAN CURR\n", y);
                least_played = current;
                leastplayedval = currval;
            }
         }
      }
     
     avgplays = avgplays / NUM_SIMULATIONS;     
    
    Song* mps = most_played->key;
    Song* lps = least_played->key;
    int* mpv = (int*)most_played->value;
    int* lpv = (int*)least_played->value;
    printf("Jukebox statistics\n");
    printf("==================\n");
    printf("Number of songs in jukebox: %zd\n", jukebox->size);
    printf("Number of simulations run: %zd\n", NUM_SIMULATIONS);
    printf("Total number of songs played: %d\n", numsongs);
    printf("Average number of plays to get a duplicate: %d\n", avgplays);
    printf("Most played song: %s by %s, %d times.\n", mps->song, mps->artist, 
            *mpv);
    printf("Least played song: %s by %s, %d times.\n", lps->song, lps->artist, 
           *lpv);
}

void runSimulation(Table* jukebox)
{
    /// seed a random number
    srand(time(0));
    long random = rand() % jukebox->size;
    //printf("RANDOM INT: %d, CAPACITY: %zd\n", random, jukebox->capacity);
    /// get an array of keys
    Song** keysarr = (Song**)keys(jukebox);
    /// run for number of sims
    for( size_t x = 0; x < NUM_SIMULATIONS; x++)
    {
        Table* current_sim = create(strHash, strEquals, longStrPrint);
        /// loop forever
        while(1)
        {
            random = rand() % jukebox->size;
            
            /// selecting an song to play randomly
            Song* randsong = keysarr[random];
            int* value = malloc(sizeof(int));
            *value = *(int*)get(jukebox, (void*)randsong) + 1;
            /// put the updated value into the table
            put(jukebox, (void*)randsong, (void*)value);

            if(!has(current_sim, randsong))
            {
                /// keeping track of number of songs before a duplicate occurs
                put(current_sim, (void*)randsong, (void*)value); 
                //value = NULL;
                //free(value);               
            }
            else
            {
                //value = NULL;
                //free(value);
                break;
            }
         }
         destroy(current_sim);
      }
      free(keysarr);
}
