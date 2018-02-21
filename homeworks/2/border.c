#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **input){
	//if user input is not suitable
	if(argc<=1){
		printf("usage: border arguments are the text to border.\n");
		return EXIT_FAILURE; //not enough args, so failure
	}
	
	//initial prompt to the user for the symbol to use
	printf("Enter the character for the border:\n");
	char symbol;
	while((symbol = getchar()) != EOF) {	//while loop allowing ctrl+d for immediate exit
		
		if(symbol == ' '){
			break; //a space cannot be a border
		}

	int length = argc-2;
	
	//to calculate number of characters to create a large enough border
	for(int k=1; k<argc; k++){
	length+=strlen(input[k]);
	}

	//printf("Number of characters: %d\n", length);
	
	//creates the top bar of symbols
	for(int i=0; i<length+4; i++){
		printf("%c", symbol);
	}

	//new line
	printf("\n");

	//blank line per requirement
	printf("%c", symbol);
	for(int i=0; i<length+2; i++){
		printf(" ");
	}
	printf("%c\n", symbol);//new line
	
	//single symbol before user phrase
	printf("%c", symbol);
	for(int i=1; i<argc; i++){
		printf(" %s",input[i]); //user phrase
	}
	
	//single symbol after user phrase
	printf(" %c\n", symbol);

	//blank line
	printf("%c", symbol);
	for(int i=0; i<length+2; i++){
		printf(" "); //blank line for border
	}
	printf("%c\n", symbol); //new line

	//creating bottom row of symbols
	for(int i=0; i<length+4; i++){
	printf("%c", symbol);
	}
	
	printf("\n");
	
	return EXIT_SUCCESS; //program fully completed, therefore successful
	break;
	}
	return EXIT_FAILURE; //program was user terminated, so failure
}
