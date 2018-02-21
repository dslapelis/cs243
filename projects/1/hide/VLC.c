#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node_heap.h"
#include <stdbool.h>

bool isValueInArray(Symbol sa[], char c, int size);
void addNewSymbol(Symbol sa[], char c);

int main(void)
{
	//create an array of symbols with a size of 4
	int arr_size = 4;
	Symbol symarray[arr_size];
	char ch;
	
	//while there are more charachters in the file
	//get the next character, check if it is already in a struct
	//in the symbol array. If it is, increase the frequency of that
	//symbol. Otherwise, create a new symbol.
	while (ch!=EOF) {
		ch = fgetc(stdin);
		if (!isValueInArray(symarray, ch, arr_size))
		{	
			addNewSymbol(symarray, ch);
		} 
		putchar(ch);
	}

	return 0;

}

bool isValueInArray(Symbol sa[], char c, int size)
{
	//printf("%c", c);
	int i;
	for (i=0; i < size; i++)
	{
		printf("Array Char: %c | Stdin Char: %c", sa[i].symbol, c);
		if(sa[i].symbol == c)
		{
			printf("\nValue is in array.\n");
			return true;
		}
	}
	printf("\nValue is not in array.\n");
	return false;
}

void addNewSymbol(Symbol sa[], char c)
{
	printf("\nAdding new symbol\n");
	int counter = 0;
	while(counter < 4)
	{
		if(sa[counter].symbol == '@')
		{
			sa[counter].symbol = c;
			printf("\nNew symbol added to symbol array\n");
			break;
		}
		counter++;
	}
}
