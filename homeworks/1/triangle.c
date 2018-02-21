/// 
/// File: triangle.c 
/// 
/// A program to print out multiple "pyramid" triangles of the form:
/// <pre>
///   *
///  ***
/// *****
/// </pre>
/// 
/// @author TODO_AUTHOR_ID_ : TODO_AUTHOR_FULL_NAME_
/// 
// // // // // // // // // // // // // // // // // // // // // // // // 

// TODO_ADD_#INCLUDES_ HERE

#include <stdio.h>

void drawTriangle(int);
/**
 *  Function: drawTriangle
 *
 *  Description: Draw a 'pyramid' triangle whose base width is 'size'.
 *  Note: If size is even, the function makes a 'size + 1' pyramid triangle. 
 * 
 *  @param size  the width of the base of the triangle to draw
 */
// TODO_WRITE_DRAWTRIANGLE_FUNCTION HERE
void drawTriangle(int size)
{
	if(size%2==0)
	{
		size = size+1;
	}

	for(int i = 1; i<size+1; i+=2)
	{
		for(int j=0; j<4-i/2; j++) //loop for spaces 
		{	
			printf(" ");
		}
		for(int k=0;k<i;k++) //loop for printing the asteriks
		{
		printf("*");
		}
		printf("\n"); //make a new line
	}
}
/**
 *  Function: main 
 *
 *  Description: draws pyramid triangles of size 1, 5, and '6'.
 *
 *  @returns errorCode  error Code; EXIT_SUCCESS if no error
 */
// TODO_WRITE_MAIN_FUNCTION HERE
int main()
{
drawTriangle(1);
drawTriangle(5);
drawTriangle(6);

return 0;
}
// // // // // // // // // // // // // // // // // // // // // // // // 
// Version: 
// $Id$ 
// 
// Revisions: 
// $Log$ 
