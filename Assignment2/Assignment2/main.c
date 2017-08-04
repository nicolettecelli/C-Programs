#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "bmplib.h"

/*
 * 	Nicolette Celli
 * 	4174075
 * 	I affirm that this program is entirely my own work and none of it is the work of any other person.
 */

// This program can manipulate (enlarge, rotate, and flip) a BMP image by using command line arguments.

/*
 * This method enlarges a 24-bit, uncompressed .bmp file
 * that has been read in using readFile()
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the original number of rows
 * cols     - the original number of columns
 *
 * scale    - the multiplier applied to EACH OF the rows and columns, e.g.
 *           if scale=2, then 2* rows and 2*cols
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows (scale*rows)
 * newcols  - the new number of cols (scale*cols)
 */
int enlarge(PIXEL* original, int rows, int cols, int scale, 
	    PIXEL** new, int* newrows, int* newcols) 
{
	*newrows = rows * scale;
	*newcols = cols * scale;

	*new = (PIXEL *)malloc((*newrows) * (*newcols) * sizeof(PIXEL)); 

	int x, i, j, sc;

	for (x = 0; x < *newrows; x++)
	{
		sc = x / scale;
		PIXEL *o = original + (sc * cols);
		PIXEL *n = *new + (x * (*newcols));

		for (i = 0; i < cols; i++)
		{
			for (j = 0; j < scale; j++)
			{
				*n++ = *o;
			}
			*o++;
		}
	}
	
	return 0;
}

/*
 * This method rotates a 24-bit, uncompressed .bmp file that has been read 
 * in using readFile(). The rotation is expressed in degrees and can be
 * positive, negative, or 0 -- but it must be a multiple of 90 degrees
 * 
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 * rotation - a positive or negative rotation, 
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows
 * newcols  - the new number of cols
 */
int rotate(PIXEL* original, int rows, int cols, int rotation,
	   PIXEL** new, int* newrows, int* newcols)
{
	int i, row, col, rotationTimes;

	if ((rows <= 0) || (cols <= 0))
		return -1;

	if ((rotation % 90) != 0)
	{
		printf("Rotation must be a multiple of 90.\n");
		exit(1);
	}
	else if (rotation < 0)	// Counterclockwise rotation
	{
		rotationTimes = rotation / 90;

		for (i = 0; i < rotationTimes; i++)
		{
			*new = (PIXEL *)malloc(rows * cols * sizeof(PIXEL));

			for (row = 0; row < rows; row++)
			{
				for (col = 0; col < cols; col++)
				{
					PIXEL *o = original + (row * cols) + col;
					PIXEL *n = (*new) + ((cols - 1 - col) * rows) + row;
					*n = *o;
				}
			}
			original = *new;
		}
	}
	else	// Clockwise rotation
	{
		rotationTimes = abs(rotation) / 90;

		for (i = 0; i < rotationTimes; i++)
		{
			*new = (PIXEL *)malloc(rows * cols * sizeof(PIXEL));

			for (row = 0; row < rows; row++)
			{
				for (col = 0; col < cols; col++)
				{
					PIXEL *o = original + (row * cols) + col;
					PIXEL *n = *new + (col * rows) + (rows - 1 - row);
					*n = *o;
				}
			}
			original = *new;
		}
	}

	return 0;
}

/*
 * This method horizontally flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile(). 
 * 
 * THIS IS GIVEN TO YOU SOLELY TO LOOK AT AS AN EXAMPLE
 * TRY TO UNDERSTAND HOW IT WORKS
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */
int flip (PIXEL *original, PIXEL **new, int rows, int cols) 
{
  int row, col;

  if ((rows <= 0) || (cols <= 0)) return -1;

  *new = (PIXEL *)malloc(rows*cols*sizeof(PIXEL));

  for (row=0; row < rows; row++)
    for (col=0; col < cols; col++) {
      PIXEL* o = original + row*cols + col;
      PIXEL* n = (*new) + row*cols + (cols-1-col);
      *n = *o;
    }

  return 0;
}

int main(int argc, char **argv)
{
  int r, c;
  PIXEL *b, *nb;

  // Save the dimensions of the altered image
  int *newRows = (int *)malloc(sizeof(int));
  int *newCols = (int *)malloc(sizeof(int));

  int o;
  int sflag = 0;
  int rflag = 0;
  int fflag = 0;
  int oflag = 0;
  int rotation, scale;

  char *infile; 	// Image to be altered
  char *outfile;	// Altered image

  opterr = 0;

//  printf("Usage: ./bmptool [-s scale | -r degree | -f ] [-o output_file] [input_file] \n");
  while ((o = getopt(argc, argv, "s:r:fo:")) != -1)
	  switch (o)
	  {
		case 's':
			sflag++;
			scale = atoi(optarg);
			break;
		case 'r':
			rflag++;
			rotation = atoi(optarg);
			break;
		case 'f':
			fflag++;
			break;
		case 'o':
			oflag++;
			outfile = optarg;
			break;
		case '?':
			if (optopt == 's' || optopt == 'r' || optopt == 'o')
				printf("Option -%c requires an argument.\n", optopt);
			else if (isprint(optopt))
				printf("Unknown option '-%c'\n", optopt);
			else
				printf("Unknown option character '\\x%x'\n", optopt);
			
			printf("Usage: ./bmptool [-s scale | -r degree | -f ] [-o output_file] [input_file] \n");
			exit(1);	// error
		default:
			abort();
	  }

  // Input file is specified
  if (optind == (argc - 1))
	  infile = argv[optind];

/*  else // No input file given, so read from standard input
	  infile = stdin;
*/
  // No output file given, so write to standard output
/*  if (oflag == 0)
	  outfile = stdout;
*/
  if (sflag == 1)
  {
	  readFile(infile, &r, &c, &b);
	  enlarge(b, r, c, scale, &nb, newRows, newCols);
	  writeFile(outfile, *newRows, *newCols, nb);
	  infile = outfile;
  }
  if (rflag == 1)
  {
	  readFile(infile, &r, &c, &b);
	  rotate(b, r, c, rotation, &nb, newRows, newCols);
	  writeFile(outfile, r, c, nb);
	  infile = outfile;
  }
  if (fflag == 1)
  {
	  readFile(infile, &r, &c, &b);
	  flip(b, &nb, r, c);
	  writeFile(outfile, r, c, nb);
	  infile = outfile; 
 }


  /*
  readFile("example.bmp", &r, &c, &b);
  flip(b, &nb, r, c);
  writeFile("result.bmp", r, c, nb);*/
    
  free(b);
  free(nb);
  free(newRows);
  free(newCols);

  return 0;
}
