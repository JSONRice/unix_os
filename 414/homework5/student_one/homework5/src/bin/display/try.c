
#include <stdio.h>

#define ROWS_SIZE	24
#define COLS_SIZE	80
#define SHM_SIZE	(2 * ROWS_SIZE * 2 * COLS_SIZE * sizeof (int))

typedef int MAPS [2*ROWS_SIZE][2*COLS_SIZE];

MAPS	*Display_map;
/*
static char (*Display_map)[2*ROWS_SIZE][2*COLS_SIZE];
*/

main(argc, argv)
int argc;
char *argv[];
{
	int row, col;

	/* parse arguments */

	if ((Display_map = (MAPS *) malloc (SHM_SIZE)) < (MAPS *) NULL)
	{
		perror ("shmat");
		exit (3);
	}
	for (row = 0; row < 2 * ROWS_SIZE; row++)
		for (col = 0; col < 2 * COLS_SIZE; col++)
		{
			(*Display_map)[row][col] = row * col;
		}
	printf ("7, 9 = %d\n", (*Display_map)[7][9]);
	printf ("9, 9 = %d\n", (*Display_map)[9][9]);
	printf ("20, 13 = %d\n", (*Display_map)[20][13]);
	exit (0);
}
