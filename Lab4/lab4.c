#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*
	Nicolette Celli
	4174075
	I affirm that this program is entirely my own work and none of it is the work of any other person.
*/

// This program reads a file or user input to determine the largest and shortest word in a string.
int main(int argc, char **argv)
{
	FILE *fp;
	char buffer[100];
	char largest[100];
	char smallest[100];

	int fflag = 0;
	
	int c;
	opterr = 0;

	printf("Usage: ./lab4 [-f filename]\n\n");

	while ((c = getopt(argc, argv, "f:")) != -1)
		switch (c)
		{
			case 'f':
				fflag = 1;		// File is specified
				fp = fopen(optarg, "r");

				if (fp == NULL)	// File cannot be opened
				{
					printf("The file %s cannot be opened.\n", optarg);
					return -1;
				}
				break;
			case '?':
				if (optopt == 'f')
					printf("Option -f requires an argument.\n");
				else if (isprint(optopt))
					printf("Unknown option -%c\n", optopt);
				else
					printf("Unknown option character \\x%x\n", optopt);

				printf("Usage: ./lab4 [-f filename]\n");
				return -1;
			default:
				abort();
		}

	if (fflag == 0)		// File is not specifed
	{
		fp = stdin;		
		printf("Please indicate EOF with \"~\" on a new line.\n\n");
	}

	strcpy(largest, "");
	strcpy(smallest, "abcdefghijklmnopqrstuvwxyz");

	if (fflag == 1)		// Reading from a file
	{
		while (fscanf(fp, "%s", buffer) == 1)	// Not EOF
		{
			if ((largest == NULL) || (strlen(buffer) > strlen(largest)))
				strcpy(largest, buffer);

			if ((smallest == NULL) || (strlen(buffer) < strlen(smallest)))
				strcpy(smallest, buffer);
		}
	}
	else	// Reading from stdin
	{
		while ((fscanf(fp, "%s", buffer) == 1) && (buffer[0] != '~'))	// ~ should be inputted by user to indicate EOF
		{
			if ((largest == NULL) || (strlen(buffer) > strlen(largest)))
				strcpy(largest, buffer);

			if ((smallest == NULL) || (strlen(buffer) < strlen(smallest)))
				strcpy(smallest, buffer);
		}
	}

	printf("Largest word: %s\n", largest);
	printf("Smallest word: %s\n", smallest);

	return 0;
}
