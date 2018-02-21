//
// file: crypt.c
// author: Sam Fryer, smf@cs.rit.edu
//
//# performs a simple XOR encryption/decryption 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LENGTH 1024
#define MAX_BUFF_LENGTH 1024

/// takes key, source, and output files.  
/// Performs XOR on source and key saving the result in the output file.
///
int main(int argc, char * argv[])
{
	size_t i,j;
	size_t bufLength = 0;
	size_t keyLength = 0;
	char key[MAX_KEY_LENGTH];
	char buf[MAX_BUFF_LENGTH];
	FILE * keyfile = NULL;
	FILE * fin = NULL;
	FILE * fout = NULL;

	if (argc == 4)
	{

		// Open the files
		keyfile = fopen(argv[1],"rb");
		fin = fopen(argv[2],"rb");
		fout = fopen(argv[3],"wb");

		if ((keyfile != NULL) && (fin != NULL) && (fout != NULL))
		{
			// read in the key
			keyLength = fread(key,1,MAX_KEY_LENGTH,keyfile);
			fclose(keyfile);

			// read the data, perform the XOR and write out	
			i=0;
			while((bufLength = fread(buf,1,MAX_BUFF_LENGTH,fin))>0)
			{
				for(j=0;j<bufLength;i++,j++)
				{
					if (i>=keyLength) i = 0;
					buf[j] ^= key[i];
				}
				fwrite(buf,1,bufLength,fout);
			}

			// close up
			fclose(fin);
			fclose(fout);
		}
		else
			printf("Unable to open one of the files.\n");
	}
	else
		printf("usage: %s <keyfile> <inputfile> <outputfile>\n",argv[0]);


	return 0;
}

