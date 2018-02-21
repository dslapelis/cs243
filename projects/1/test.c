#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "node_heap.h"

//checks if value is in array -- pass through char and the symbol
//array
bool isValInArray(char c, Symbol sym_array[], int s);

//add the char c to the array of symbols
void addValToArray(char c, Symbol sym_array[], int s);

int main(void)
{

	int max_size = 0;
	char c = fgetc(stdin);
	char char_array[];
	while( c != EOF)
	{
		c = fgetc(stdin);
		char_array[max_size]=c;
		max_size += 1;
	}
	//create a starting array -- fill it with blank symbols
	Symbol array[max_size];
	for(int i = 0; i<max_size; i++)
	{
		array[i].symbol = '@';
	}

	//read file char by char
	//check if char exists within a symbol object already
	//if not, create a new object. if true, increase freq
	//of that symbol by one.
	char ch = fgetc(stdin);

	while(ch != EOF)
	{
		//putchar(ch);
		if(!isValInArray(ch, array, max_size))
		{
			addValToArray(ch, array, max_size);
		}
		ch = fgetc(stdin);
	}
	
	
	printf("FINAL CHARACTERS: ");
	for (int i =0; i < 10; i++)
	{
		printf("CHAR: %c FREQ: %zu\n", array[i].symbol,
		array[i].frequency);
	}

	return 0;
}

bool isValInArray(char c, Symbol sym_array[], int s)
{
	int count = 0;
	while (count < s)
	{
		char temp = sym_array[count].symbol;
		printf("---- STDIN CHAR IS %c | TEMP CHAR IS %c ----\n", c, temp);
		if (sym_array[count].symbol == c)
		{
			printf("%c is in array!\n", c);
			sym_array[count].frequency+=1;
			return true;
			break;
		}
		count++;
	}
	printf("%c is not in array!\n", c);
	return false;
}

void addValToArray(char c, Symbol sym_array[], int s)
{	
	int count = 0;
	while(count < s)
	{
		if(sym_array[count].symbol == '@')
		{	
			printf("New value added to array (%c)\n", c);
			sym_array[count].symbol = c;
			sym_array[count].frequency = 1;
			break;
		}
		count++;
	}
}
