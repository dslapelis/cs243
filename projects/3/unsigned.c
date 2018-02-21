#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    unsigned char c = 50;
    unsigned int i = 0;
    
    i = (unsigned int) c;
  
    printf("Char %c is %u\n", c, i);

    return 0;
}
