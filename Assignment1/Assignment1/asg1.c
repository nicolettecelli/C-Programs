#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

/*
	Nicolette Celli
	4174075
	I affirm that this program is entirely my own work and none of it is the work of any other person.
*/

// This program reads command line inputs and uses a binary search tree to return the user's input in alphabetical order.

struct bstNode
{
	char string[100];	
	// children
	struct bstNode *left;
	struct bstNode *right; 
};
typedef struct bstNode BstNode;

// Inserts a node in a binary search tree (case sensitive)
void insertNode(BstNode **ptr, char input[100])
{
	if (*ptr == NULL)	// Tree is empty or we have reached a leaf
	{
		*ptr = (BstNode *)malloc(sizeof(BstNode));	// Allocation

		strcpy((*ptr)->string, input);
		(*ptr)->left = NULL;
		(*ptr)->right = NULL;
	}
	else if (stringCmp(input, (*ptr)->string) <= 0)
		insertNode(&((*ptr)->left), input);
	else if (stringCmp(input, (*ptr)->string) > 0)
		insertNode(&((*ptr)->right), input);
}
// Inserts a node in a binary search tree (case insensitive)
void insertNodeCase(BstNode **ptr, char input[100])
{
printf("inside insertNodeCase\n");
	if (*ptr == NULL) // Tree is empty or we have reached a leaf
	{
printf("inside null loop\n");
printf("before malloc\n");
		*ptr = (BstNode *)malloc(sizeof(BstNode));	// Allocation
printf("after malloc\n");	

printf("before strcpy\n");	
		strcpy((*ptr)->string, input);
printf("after strcpy: %s\n", (*ptr)->string);
		(*ptr)->left = NULL;
		(*ptr)->right = NULL;
	}
	else if (stringCaseCmp(input, (*ptr)->string) <= 0)
		insertNode(&((*ptr)->left), input);
	else if (stringCaseCmp(input, (*ptr)->string) > 0)
		insertNode(&((*ptr)->right), input);
printf("end insertNodeCase\n");
}

// Destroys a binary search tree
void destroyTree(BstNode *leaf)
{
	if (leaf != NULL)	// Removes and deallocates nodes from the leaves to the root
	{
		destroyTree(leaf->left);
		destroyTree(leaf->right);

		free(leaf);	// Deallocation
	}
}

// Prints the contents of a binary search tree in alphabetical order
void printTree(BstNode *ptr, FILE *output)
{
	if (ptr != NULL) 
	{
		printTree(ptr->left, output);	// Starts leftmost leaf 
		fprintf(output, ptr->string);	// Prints data
		printTree(ptr->right, output);	// Ends at rightmost leaf
	}
}

// Compares case-sensitive strings
// Returns a negative number if str1 < str2, 0 if they are equal, or a positive number if str1 > str2
int stringCmp(char *str1, char *str2)
{
	while (*str1 == *str2)	
	{
		if (*str1 == '\0' || *str2 == '\0')	// If either string terminates, exit loop
			break; 
		str1++;
		str2++;
	}

	if (*str1 == '\0' && *str2 == '\0')	// Both strings are the same
		return 0;
	else if (*str1 == '\0' && *str2 != '\0')	// str1 is shorter than str2
		return -1;
	else if (*str1 != '\0' && *str2 == '\0')	// str1 is longer than str2
		return 1;
	else
		return (*str1) - (*str2);	// The characters are different
}

// Compares case-insensitive strings
// Returns a negative number if str1 < str2, 0 if they are equal, or a positive number if str1 > str2
int stringCaseCmp(char *str1, char *str2)
{
	int diff; 

	while (tolower(*str1) == tolower(*str2))
	{
		if (*str1 == '\0' || *str2 == '\0')
			break;
		str1++;
		str2++;
 	}

	if (*str1 == '\0' && *str2 == '\0')	// Both strings are the same
		return 0;
	else if (*str1 == '\0' && *str2 != '\0')	// str1 is shorter than str2
		return -1;
	else if (*str1 != '\0' && *str2 == '\0')	// str1 is longer than str2
		return 1;
	else
		// Characters are different
		return tolower(*str1) - tolower(*str2);
	
}

int main(int argc, char **argv)
{
	FILE *infile;	// input file
	FILE *outfile;	// output file

	int caseSensitive = 0;
	int oflag = 0;
	int iflag = 0;
	int c;

	opterr = 0;
	
	while ((c = getopt(argc, argv, "co:")) != -1)
		switch (c)
		{
			case 'c':
				caseSensitive = 1;
printf("c here\n");
				break;
			case 'o':
				oflag = 1;	
printf("before outfile open\n");
				outfile = fopen(optarg, "w");
printf("after outfile open\n");
				if (outfile == NULL)
				{
					printf("File %s cannot be opened.\n", optarg);
					exit(1);	// error
				}
				break;
			case '?':
				if (optopt == 'o')
					printf("Option -%c requires an argument.\n", optopt);
				else if (isprint(optopt))
					printf("Unknown option '-%c'\n", optopt);
				else
					printf("Unknown option character '\\x%x'\n", optopt);

				printf("Usage: ./bstsort [-c] [-o output_file_name] [input_file_name]\n");	// Always print usage statement
				exit(1);	// error
			default:
				abort();
		}
printf("after getopt\n");

	// Input file is specified
	if (optind == (argc - 1))
	{
		iflag = 1;
printf("infile arg specified\n");
		infile = fopen(argv[optind], "r");

		if (infile == NULL)
		{
			printf("File %s cannot be opened.\n", argv[optind]);
			exit(1);	// error
		}
	}
printf("after infile w arg\n");
	// None of the above conditions were met, so read from standard input
	if (iflag == 0)
	{
		infile = stdin;
printf("infile = stdin\n");
	}

	//  None of the above conditions were met, so write to standard output
	if (oflag == 0)
	{
		outfile = stdout;
printf("outfile = stdout\n");
	}

	BstNode **root;
	root = (BstNode **)malloc(sizeof(BstNode *));
	char line[100];
printf("node and buffer defined.\n");

	if (caseSensitive == 1)
	{
printf("inside cs loop\n");
		if (iflag == 1)		// Reading from a file
		{
			while (fgets(line, 100, infile) != NULL)
			{
printf("before cs node insert\n");
				insertNode(root, line);
printf("before cs node insert\n");
			}
		}
		else	// Reading from stdin
		{
			while ((fgets(line, 100, infile) != NULL) && (line[0] != '\n'))
			{
printf("before stdin cs node insert\n");	
				insertNode(root, line);
printf("after stdin cs node insert\n");
			}
		}
	}
	else
	{
//printf("inside cis loop\n");
		if (iflag == 1)		// Reading from a file
		{
			while (fgets(line, 100, infile) != NULL)
			{
printf("before cis node insert\n");
				insertNodeCase(root, line);
printf("after cis node insert\n");
			}
		}
		else	// Reading from stdin
		{
			while ((fgets(line, 100, infile) != NULL) && (line[0] != '\n'))
			{
printf("before stdin cis node insert\n");	
				insertNodeCase(root, line);
printf("after stdin cis node insert\n");
			}
		}
	}


printf("all input processed\n");
	fclose(infile);
printf("infile close\n");


	printTree(*root, outfile);
printf("tree printed\n");

	if (oflag = 1)
		fclose(outfile);
printf("outfile closed\n");
	
	destroyTree(*root);
printf("tree destroyed\n");
	
	return 0;
}
