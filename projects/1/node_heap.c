/*
* Author: Daniel Slapelis
* ID: djs7209
* Date: 3/4/15
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "node_heap.h"

int unique_characters;

bool check_existance( size_t maxcount, Symbol syms[], char c );
void populate_array( size_t maxcount, Symbol syms[] );



/// read_symbols reads characters from standard input, calculates
/// each symbol's frequency of appearance, and counts the number of
/// distinct symbols it sees.
/// @pre: maxcount is a the capacity of the syms array.
/// @post: syms array is initialized and filled with histogram information.
/// @post: syms is ordered so that all used entries are before unused.
/// @post: unused syms array entries are initialized as unused.
///

int read_symbols( size_t maxcount, Symbol syms[] )
{

	//printf("Inside of read_symbols\n");
	unique_characters = 0;	

	populate_array( maxcount, syms );

	char c = fgetc(stdin);
	//printf("FIRST CHAR IS %c\n", c);
	while( c != EOF )
	{
		//printf("CURRENT CHAR IS %c\n", c);
		//printf("Inside of read_symbols while loop\n");
		if( !check_existance( maxcount, syms, c ))
		{
			//printf("%c does not exist\n", c);
			/// since c doesnt exist, we add 1 to our unique
			/// characters count.
			unique_characters++;
			for( size_t i = 0; i < maxcount; i++)
			{
				/// if a symbols frequency is 0, we
				/// know that it is unused.
				if( syms[i].frequency == 0 )
				{
					/// new symbol at i
					/// add one to its freqeuncy
					syms[i].symbol = c;
					syms[i].frequency++;
					break;
				}
			}
		}
		else
		{
			for( size_t i = 0; i < maxcount; i++ )
			{
				if( syms[i].symbol == c )
				{
					syms[i].frequency++;
				}
			}
		}

		c = fgetc(stdin);
	}

	//printf("UNIQUE CHARACTERS: %d\n", unique_characters);
 	
	/// sorting the beautiful symbol array from largest freq
	/// to smallest freq (so used to unused)
	for( size_t i = 0; i < maxcount-1; i++ )
	{
   		for( size_t j = 1; j < maxcount - i; j++ )
		{
			if( syms[j-1].frequency < syms[j].frequency )
			{
         			Symbol temp = syms[j-1];
        			syms[j-1] = syms[j];
        			syms[j] = temp;
      			}
   		}
	}
		

	return unique_characters;
}	


/// heap_init initializes the heap storage with all unused Node entries.
/// heap is a <em>pointer</em>, a reference to a heap structure.
/// @pre: heap is a valid pointer and heap->capacity is uninitialized.
/// @post: heap->array has been initialized with unused Node entries.
/// @post: heap->size == 0.
/// @post: heap->capacity is initialized to the capacity of heap->array.
///

void heap_init( Heap * heap )
{
	//heap->array[unique_characters];
	heap->size = 0;
	heap->capacity = NSYMS;
	for( size_t i = 0; i < heap->capacity; i++)
	{
		heap->array[i].frequency = 0;
		heap->array[i].num_valid = 0;
	}
}

/// heap_make fills heap with symbols from symlist and <em>heapifies</em> it.
/// heap is a <em>pointer</em>, a reference to an initialized heap.
/// length is the length of the symlist.
/// <p>algorithm:
/// Take each symbol from symlist and put it into the
/// next note in the heap, initializing the node's frequency
/// to that of the symbol, and setting the node's num_valid to 1.
/// @pre: entries in symlist[0, ..., length-1] are valid Symbol objects.
/// @pre: heap is an initialized, unused heap of capacity >= length.
/// @pre: length <= heap->capacity. symlist is array of Symbol structs.
/// @post: heap->array is filled with Node entries in min-heap order.
/// @post: heap->size == length.
/// @post: heap->array[i].syms[0] is valid symbol for i in [0, ..., length-1].
///

void heap_make( Heap * heap, size_t length, Symbol symlist[] )
{	
	if( heap->capacity >= length)
	{
	heap->size = length;
	for( size_t i = 0; i < length; i++ )
	{
		/// modify the node in the heap
		Node temp;
		temp.syms[0] = symlist[i];
		/// add symbol to position 0 in syms
		/// obtain the value of the frequency of symbol
		temp.frequency = symlist[i].frequency;
		temp.num_valid = 1;
		heap->array[heap->size] = temp;
		heap->size++;
		size_t pos = heap->size;
		while( pos > 0 )
		{
			if( heap->array[pos].frequency < 
				heap->array[(pos-1)/2].frequency )
			{
				Node temp2 = heap->array[(pos-1)/2];
				heap->array[(pos-2)/2] = heap->array
					[pos];
				heap->array[pos] = temp2;
				pos = (pos-1)/2;
			}
			else
			{
				break;
			}
		}
	}
	}
}


/// heap_add adds one more node to the current heap.
/// heap_add replaces what is expected to be an unused Node entry.
/// heap is a <em>pointer</em>, a reference to an initialized heap.
/// @pre: heap->size < heap->capacity (fatal error to exceed heap capacity)
/// @post: heap->size has increased by 1.
/// @post: heap->array is in heap order.
/// <p>algorithm:
/// Since the heap->array is fixed capacity, heap_add cannot enlarge it;
/// instead the add overwrites an unused node in the heap structure.
/// The add puts the node's values into the next location in the heap's array.
/// <p>
/// After assigning the values, heap_add restores heap order
/// by sifting the last entry up the heap from the last location.
/// Finally it increments the size of the heap.
/// <p>
/// If there is no room for more nodes, then it is a fatal error.
/// However, the array is sized to hold enough nodes for all the symbols
/// it can encounter. (Running out of nodes would indicate a programming
/// design error.)
///

void heap_add( Heap * heap, Node node )
{
	heap->array[heap->size] = node;
	size_t pos = heap->size;
	while( pos > 0 )
	{
		if( heap->array[pos].frequency < heap->array[(pos-1)/2].
			frequency)
		{
			Node temp = heap->array[(pos-1)/2];
			heap->array[(pos-1)/2] = heap->array[pos];
			heap->array[pos] = temp;
			pos = (pos-1)/2;
		}
		else
		{
			break;
		}
	}
	heap->size = heap->size+1;
}/// end function


/// heap_remove removes and returns a node structure.
/// heap is a <em>pointer</em>, a reference to an initialized heap.
/// @pre: heap->size > 0 (fatal error to remove from an empty heap)
/// @post: heap->size has decreased by 1.
/// @post: remaining heap is in proper heap order.
/// <p>algorithm:
/// Save the top of the heap at index 0.
/// Reduce the size of the heap by
/// copying the last heap entry into the top location and
/// replacing the last location's entry with values representing an
/// unused Node object.
/// Restore the heap order by sifting the top entry down the heap.
/// Return

Node heap_remove( Heap * heap )
{
	Node answer;
	if( heap->size > 0 )
	{
		answer = heap->array[0];
		heap->array[0] = heap->array[heap->size-1];
		size_t num = 0;
		while( num < heap->size/2 )
		{
			if( num == 0 )
			{
				if( heap->array[num].frequency > heap->array[num+1].frequency ||
						heap->array[num].frequency > heap->array[num+2].frequency )
				{
					if( heap->array[num+1].frequency <= heap->array[num+2].frequency )
					{
						Node temp = heap->array[num+1];
						heap->array[num+1] = heap->array[num];
						heap->array[num] = temp;
					}
					else
					{
						Node temp = heap->array[num+2];
						heap->array[num+2] = heap->array[num];
						heap->array[num] = temp;
					}
				}
			}
			else
			{
				if( heap->array[num].frequency > heap->array[num*2].frequency ||
						heap->array[num].frequency > heap->array[(num*2)+1].frequency )
				{
					if( heap->array[num*2].frequency <= heap->array[(num*2)+1].frequency )
					{
						Node temp = heap->array[num*2];
						heap->array[num*2] = heap->array[num];
						heap->array[num] = temp;
					}
					else
					{
						Node temp = heap->array[(num*2)+1];
						heap->array[(num*2)+1] = heap->array[num];
						heap->array[num] = temp;
					}
				}
			}
			num++;
		}
		heap->size = heap->size-1;
		return answer;
	}
	return answer;
}

//-----AUTHOR DEFINED FUNCTIONS-----

bool check_existance( size_t maxcount, Symbol syms[], char c )
{
	for( size_t i = 0; i < maxcount; i++ )
	{
		if( syms[i].symbol == c )
		{
			return true;
		}
	}
	return false;
}

void populate_array( size_t maxcount, Symbol syms[] )
{	
	int count = 0;
	for( size_t i = 0; i < maxcount; i++ )
	{
		//printf("POPULATING ARRAY AT %d\n", count);
		syms[i].frequency = 0;
		count++;
	}
}
