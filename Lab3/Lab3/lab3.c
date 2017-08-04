#include <stdio.h>
#include <stdlib.h>

/*
	Nicolette Celli
	4174075
	I affirm that this program is entirely my own work and none of it is the work of any other person.
*/

static int count = 1;

/*
	Counts how many times main is called.
*/
int main(int argc, char** argv)
{
	if (argc != 2) // No arguments or too many arguments inputted
	{
		printf("The command takes one argument.\n");	
		return 0;
	}

	// Takes argument loopcount, which is the number of calls to main	
	int c = atoi(argv[1]);	

	if (c < 0) // Argument is a negative integer
	{
		printf("The argument must be a positive integer.\n");
		return 0;
	}
	else if (c == 0) // Argument is 0 or the program has run loopcount times
	{
		printf("The program has finished running.\n");
		return 0;
	}
	else if (c > 0) 
	{
		printf("Count: %d\n", count);
		count++;

		char* decrement = (char*)malloc(1);
		sprintf(decrement, "%d", c - 1); // Converts int to string
		argv[1] = decrement; // Decrements the count in the argument

		main(2, argv); // Recursive call
	}	
}
