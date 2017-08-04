#include <stdio.h>

/*
	Nicolette Celli
	4174075
	I affirm that this program is entirely my own work and none of it is the work of any other person.
*/

// Prints a list of items.
int main()
{
	// Variables
	int item1 = 34523;
	char desc1[] = "Bookshelf";
	double price1 = 78.5;
	int count1 = 4;

	int item2 = 747482;
	char desc2[] = "Pen";
	double price2 = 2.99;
	int count2 = 100;

	int item3 = 98754;
	char desc3[] = "Chair";
	double price3 = 129.99;
	int count3 = 6;

	int item4 = 234234;
	char desc4[] = "Camera";
	double price4 = 1295.4;
	int count4 = 3;

	// Additional item
	int item5 = 839292;
	char desc5[] = "Table";
	double price5 = 150.5;
	int count5 = 15;

	// Print headers
	printf("\nItem Num\t%-20s Price\tCount\tExtended Price\n", "Description");

	// Print statements for each line
	printf("%08d\t%s\t%-4s %-15.2f%-8d%-3s %.2f\n", item1, desc1, "$", price1, count1, "$", price1 * count1);
	printf("%08d\t%s\t\t%-5s %-12.2f%-10d%-3s %.2f\n", item2, desc2, "$", price2, count2, "$", price2 * count2);
	printf("%08d\t%s\t\t%-3s %-16.2f%-8d%-3s %.2f\n", item3, desc3, "$", price3, count3, "$", price3 * count3);
	printf("%08d\t%s\t\t%-2s %-17.2f%-8d%-2s %.2f\n", item4, desc4, "$", price4, count4, "$", price4 * count4);
	// Additional item
	printf("%08d\t%s\t\t%-3s %-15.2f%-9d%-2s %.2f\n\n", item5, desc5, "$", price5, count5, "$", price5 * count5); 

	return 0;
}