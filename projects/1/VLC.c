/*
*Author: Daniel Slapelis
*ID: djs7209
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "node_heap.h"

int main()
{
	/// Read symbols from STDIN
	Symbol syms[NSYMS];
	int unique_chars = read_symbols( NSYMS, syms );
	
	printf("VLC.c UNIQUE CHARS %d\n", unique_chars);
	for( int i = 0; i < NSYMS; i++ )
	{
		printf("CHAR: %c ||| FREQ: %zd\n", syms[i].symbol, syms[i].frequency);
	}

	
	/// twerks
	Heap heap;
	
	/// also twerks
	heap_init( &heap );
	
	/// doesnt wanna twerk
	heap_make( &heap, unique_chars, syms );
	
	
	Node smallest;
	Node sec_smallest;
	while( heap.size > 1 )
	{
		printf("HEAP SIZE: %zd\n", heap.size);
		smallest = heap_remove( &heap );
		sec_smallest = heap_remove( &heap );
		

		printf("Smallest num valid: %zd\n", smallest.num_valid);
		printf("Second smallest num valid %zd\n", sec_smallest.num_valid);	
		// for smallest
		/*for( size_t i = 0; i < smallest.num_valid; i++ )
		{
			Symbol tempsym = smallest.syms[i];
			tempsym.codeword[tempsym.bit] = '0';
			tempsym.bit++;
			//printf("Smallest chars: %c\n", tempsym.symbol);
		}*/	
		
		// for sec_smallest
		/*for( size_t i = 0; i < sec_smallest.num_valid; i++ )
		{
			Symbol tempsym = sec_smallest.syms[i];
			//printf("Second smallest chars: %c\n",tempsym.symbol);
		}*/

		for( size_t i = 0; i < sec_smallest.num_valid; i++ )
		{
			Symbol temp = sec_smallest.syms[i];
			smallest.syms[smallest.num_valid+i] = temp;
			printf("CHAR: %c\n", temp.symbol);
		}
		
		smallest.num_valid += sec_smallest.num_valid;
		
		//printf("NUM VALID: %zd\n", smallest.num_valid);

		smallest.frequency += sec_smallest.frequency;

		//printf("FREQ: %zd\n", smallest.frequency);
		
		heap_add( &heap, smallest);	
	}// emd while
		
		printf("FINAL NUM_VALID %zd\n", heap.array[0].num_valid);

}
