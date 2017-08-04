#include <stdio.h>
#include <string.h>

/*
 * 	Nicolette Celli
 * 	4174075
 * 	I affirm that this program is entirely my own work and none of it is the work of any other person.
*/

// This program writes my name to the screen or to a specified file.
int main(int argc, char** argv )
{	
	if (argc > 1)	// If a file is specified
	{
		FILE* fp;
	
		if (strcmp(argv[1], "-f")  == 0)	// Write
		{
			fp = fopen(argv[2], "w");

			if (fp == NULL)
			{
				printf("File cannot be opened.\n");
				return 0;
			}
	
			fprintf(fp, "Nicolette Celli\n");
			fclose(fp);
		}
		else if (strcmp(argv[1], "cat") == 0)	// Append
		{
			fp = fopen(argv[2], "a");

			if (fp == NULL)
			{
				printf("File cannot be opened.\n");
				return 0;
			}
			
			fprintf(fp, "Nicolette Celli\n");
			fclose(fp);
		}
	
		return 0;
	}
	else	// Print to screen 
	{
		printf("Nicolette Celli\n");
	}

	return 0;
}
