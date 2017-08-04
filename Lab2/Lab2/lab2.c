#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
	Nicolette Celli
	4174075	
	I affirm that this program is entirely my own work and none of it is the work of any other person.
*/

int main(int argc, char **argv)
{
//	extern char * optarg;
//	extern int optind;
	int aflag = 0, dflag = 0, mflag = 0, sflag = 0, eflag = 0;
	int err = 0;
	int c;
	static char usage[] = "Usage: minicalc [-a num] [-d num] [-m num] [-s num] [-e] value";

	while ((c = getopt(argc, argv, "e:m:d:a:s")) != -1)
		switch (c) 
		{
			case 'e':
				eflag = 1;
				break;
			case 'm':
				mflag = 1;
				break;
			case 'd':
				dflag = 1;
				break;
			case 'a':
				aflag = 1;
				break;
			case 's':
				sflag = 1;
				break;
			case '?':
				err = 1;
				break;
			default:
				printf("\n\n%s\n\n", usage);
				break;
		}

    int flagsum = eflag + mflag + dflag + aflag + sflag;
        
	if (flagsum > 1)
	{
		if (aflag == 1 && sflag == 1)
		{
			printf("0: %s\n 1: %s\n 2: %s\n 3: %s\n 4: %s\n 5: %s\n 6: %s", argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
		
			int a1, a2, a3;
			sscanf(argv[2], "%d", &a1);
			sscanf(argv[5], "%d", &a2);
			sscanf(argv[6], "%d", &a3);
			int result = (a3 + a1) - a2;
			printf("\n\n(%d + %d) - %d = %d\n\n", a3, a1, a2, result);
		}
		else if (aflag == 1 && mflag == 1)
		{
			printf("0: %s\n 1: %s\n 2: %s\n 3: %s\n 4: %s\n 5: %s\n 6: %s", argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);

			int a1, a2, a3;
			sscanf(argv[2], "%d", &a1);
			sscanf(argv[4], "%d", &a2);
			sscanf(argv[6], "%d", &a3);
			int result = (a3 * a2) + a1;
			printf("\n\n(%d * %d) + %d = %d\n\n", a3, a2, a1, result);
		}
		else if (aflag == 1 && dflag == 1)
		{
			printf("0: %s\n 1: %s\n 2: %s\n 3: %s\n 4: %s\n 5: %s\n 6: %s", argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);

			int a1, a2, a3;
			sscanf(argv[2], "%d", &a1);
			sscanf(argv[4], "%d", &a2);
			sscanf(argv[6], "%d", &a3);
			float result = (a3 / (a2 * 1.0)) + a1;
			printf("\n\n(%d / %d) + %d = %.2f\n\n", a3, a2, a1, result);
		}
		else if (dflag == 1 && mflag == 1)
		{
			printf("0: %s\n 1: %s\n 2: %s\n 3: %s\n 4: %s\n 5: %s\n 6: %s", argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);

			int a1, a2, a3;
			sscanf(argv[2], "%d", &a1);
			sscanf(argv[4], "%d", &a2);
			sscanf(argv[6], "%d", &a3);
			float result = (a3 * a1) / (a2 * 1.0);
			printf("\n\n(%d * %d) / %d = %.2f\n\n", a3, a1, a2, result);
		}
		else if (dflag == 1 && sflag == 1)
		{
			printf("0: %s\n 1: %s\n 2: %s\n 3: %s\n 4: %s\n 5: %s\n 6: %s", argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);

			int a1, a2, a3;
			sscanf(argv[3], "%d", &a1);
			sscanf(argv[5], "%d", &a2);
			sscanf(argv[6], "%d", &a3);
			float result = (a3 / (a1 * 1.0)) - a2;
			printf("\n\n(%d / %d) - %d = %.2f\n\n", a3, a1, a2, result);
		}
		else if (mflag == 1 && sflag == 1)
		{
			printf("0: %s\n 1: %s\n 2: %s\n 3: %s\n 4: %s\n 5: %s\n 6: %s", argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);

			int a1, a2, a3;
			sscanf(argv[2], "%d", &a1);
			sscanf(argv[5], "%d", &a2);
			sscanf(argv[6], "%d", &a3);
			int result = (a3 * a1) - a2;
			printf("\n\n(%d * %d) - %d = %d\n\n", a3, a1, a2, result);
		}
		else if (aflag == 1 && eflag == 1)
		{
			printf("0: %s\n 1: %s\n 2: %s\n 3: %s\n 4: %s\n 5: %s", argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);

			int a1, a2;
			sscanf(argv[2], "%d", &a1);
			sscanf(argv[4], "%d", &a2);
			int result = (a2 * a2) + a1;
			printf("\n\n(%d^2) + %d = %d\n\n", a2, a1, result);
		}
		else if (mflag == 1 && eflag == 1)
		{
			printf("0: %s\n 1: %s\n 2: %s\n 3: %s\n 4: %s\n 5: %s", argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);

			int a1, a2;
			sscanf(argv[2], "%d", &a1);
			sscanf(argv[4], "%d", &a2);
			int result = (a2 * a2) * a1;
			printf("\n\n(%d^2) * %d = %d\n\n", a2, a1, result);
		}
		else if (dflag == 1 && eflag == 1)
		{
			printf("0: %s\n 1: %s\n 2: %s\n 3: %s\n 4: %s\n 5: %s", argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);

			int a1, a2;
			sscanf(argv[2], "%d", &a1);
			sscanf(argv[4], "%d", &a2);
			float result = (a2 * a2) / (a1 * 1.0);
			printf("\n\n(%d^2) / %d = %.2f\n\n", a2, a1, result);
		}
		else if (sflag == 1 && eflag == 1)
		{
			printf("0: %s\n 1: %s\n 2: %s\n 3: %s\n 4: %s\n 5: %s", argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);

			int a1, a2;
			sscanf(argv[2], "%d", &a1);
			sscanf(argv[4], "%d", &a2);
			int result = (a2 * a2) - a1;
			printf("\n\n(%d^2) - %d = %d\n\n", a2, a1, result);
		}
	}
	else
	{
		if (eflag == 1)
		{
			int a1;
			sscanf(argv[2], "%d", &a1);
			int result = a1 * a1;
			printf("\n\n%d^2 = %d\n\n", a1, result);
		}
		else if (mflag == 1)
		{
			int a1, a2;
			sscanf(argv[2], "%d", &a1);
			sscanf(argv[4], "%d", &a2);
			int result = a2 * a1;
			printf("\n\n%d * %d = %d\n\n", a2, a1, result);
		}
		else if (dflag == 1)
		{
			int a1, a2;
			sscanf(argv[2], "%d", &a1);
			sscanf(argv[4], "%d", &a2);
			float result = a2 / (a1 * 1.0);
			printf("\n\n%d / %d = %.2f\n\n", a2, a1, result);
		}
		else if (aflag == 1)
		{
			int a1, a2;
			sscanf(argv[2], "%d", &a1);
			sscanf(argv[4], "%d", &a2);
			int result = a2 + a1;
			printf("\n\n%d + %d = %d\n\n", a2, a1, result);
		}
		else if (sflag == 1)
		{
			int a1, a2;
			sscanf(argv[3], "%d", &a1);
			sscanf(argv[4], "%d", &a2);
			int result = a2 - a1;
			printf("\n\n%d - %d = %d\n\n", a2, a1, result);
		}
	}
	///////////////////////////////////////////////
/*
	
	if (optind < argc)
	{
		for (; optind < argc; optind++)
			printf("\n\nArgument: \"%s\"\n\n", argv[optind]);
	}
	else
		printf("\n\nNo arguments left to process.\n\n");		

*/	exit(0);

	return 0;
}	
