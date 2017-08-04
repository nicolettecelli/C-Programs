#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * 	Nicolette Celli
 * 	4174075
 * 	I affirm that this program is entirely my own work and none of it is the work of any other person.
 */

// This program counts the number of zero bits and one bits of the binary equivalent of the number the user inputted.
int main()
{
	int remainder;
	int zerobits = 0;
	int onebits = 0;
	char inputstr[20];
	int input;

	printf("\nPlease enter a number.\n");
	scanf("%s", inputstr);
	input = atoi(inputstr);	// Convert input string to an integer 

	do 
	{
		remainder = input % 2;	// 0 or 1
		input = input >>  1;	
	
		if (remainder == 0)
			zerobits++;
		else if (remainder == 1)
			onebits++;
	}
	while (input != 0);	

	printf("Number of one bits: %d\n", onebits);
	printf("Number of zero bits: %d\n", zerobits);

	printf("\nWould you like to enter another number? (Enter Y or N)\n");
	scanf("%s", inputstr);

	if (tolower(inputstr[0]) == 'y')
		main();		// Recursive call, repeat process
	else
		return 0;
}
	
		
