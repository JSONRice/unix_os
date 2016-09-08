#include <stdio.h>
#include <curses.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ROWS_SIZE	24
#define COLS_SIZE	80
#define SHM_SIZE	(2 * ROWS_SIZE * 2 * COLS_SIZE)

typedef char MAPS [2*ROWS_SIZE][2*COLS_SIZE];

MAPS	*Display_map;
/*
static char (*Display_map)[2*ROWS_SIZE][2*COLS_SIZE];
*/

main(int argc, char *argv[])
{
	extern int      opterr;	/* used by getopt(3) */
	extern char    *optarg;	/* used by getopt(3) */

	char		c;

	int not_finished = TRUE;
	int mid_row, mid_col;
	int min_row, max_row, min_col, max_col;
	int min_row_seen, max_row_seen, min_col_seen, max_col_seen;
	int row, col;
	int redraw;
	static int first_time = TRUE;
	int shmid;
	int shm_key = 0;

	/* command line option argument processing */
	opterr = 0;		/* suppress getopt(3) generated messages */
	while ((c = getopt(argc, argv, "m:")) != EOF)
		switch (c) 
		{
		  case 'm':
			shm_key = atoi(optarg);
		  default:
			break;
		}

	if (shm_key <= 0)
	{
		fprintf (stderr, "%s -m <shm_key>\n", argv[0]);
		exit (1);
	}
	/*
	 * connect to the shared memory segment
	 */
	shmid = shmget (shm_key, SHM_SIZE, 0644 | IPC_CREAT);
	if (shmid < 0)
	{
		perror ("shmget");
		exit (2);
	}
	if ((Display_map = (MAPS *) shmat (shmid, 0, 0)) == (MAPS *) -1)
	{
		perror ("shmat");
		exit (3);
	}
	initscr ();
	clear();
	refresh();
	while (not_finished)
	{
		if (first_time)
		{
			min_row = ROWS_SIZE / 2;
			max_row = min_row + ROWS_SIZE;
			min_col = COLS_SIZE / 2;
			max_col = min_col + COLS_SIZE;

			min_row_seen = max_row_seen = ROWS_SIZE;
			min_col_seen = max_col_seen = COLS_SIZE;

			first_time = FALSE;
		}

		min_row_seen = 2 * ROWS_SIZE;
		min_col_seen = 2 * COLS_SIZE;
		max_row_seen = 0;
		max_col_seen = 0;

		for (row = 0; row < 2 * ROWS_SIZE; row++)
			for (col = 0; col < 2 * COLS_SIZE; col++)
			{
				if ((*Display_map)[row][col] != 0)
				{
					if (row < min_row_seen)
						min_row_seen = row;
					if (col < min_col_seen)
						min_col_seen = col;
					if (row > max_row_seen)
						max_row_seen = row;
					if (col > max_col_seen)
						max_col_seen = col;
				}
				if ((*Display_map)[row][col] == ')')
					not_finished = FALSE;
				 
			}
		if (max_row_seen < min_row_seen)
		{
			sleep (1);
			continue;
		}

		mid_row = (max_row_seen + min_row_seen) / 2;
		mid_col = (max_col_seen + min_col_seen) / 2;

		min_row = mid_row - ROWS_SIZE/2 + 1;
		max_row = mid_row + ROWS_SIZE/2 + 1;
		min_col = mid_col - COLS_SIZE/2 + 1;
		max_col = mid_col + COLS_SIZE/2 + 1;

		if (min_row < 0)
			min_row = 0;
		if (max_row > 2 * ROWS_SIZE)
			max_row = (2 * ROWS_SIZE) - 1;

		if (min_col < 0)
			min_col = 0;
		if (max_col > 2 * COLS_SIZE)
			max_col = (2 * COLS_SIZE) - 1;


		repack_stdscr(min_row, max_row, min_col, max_col);

		refresh();
		sleep(1);
	}

	sleep (5);
	endwin();
}

repack_stdscr(int min_row, int max_row, int min_col, int max_col)
{
	int row, col;
	static int saved_min_row = -1;
	static int saved_max_row = -1;
	static int saved_min_col = -1;
	static int saved_max_col = -1;

	if (	min_row != saved_min_row ||
		max_row != saved_max_row ||
		min_col != saved_min_col ||
		max_col != saved_max_col)

		clear();
	for (row = min_row; row < max_row; row++)
		for (col = min_col; col < max_col; col++)
			if ((*Display_map)[row][col])
				mvaddch (row-min_row, col-min_col, (*Display_map)[row][col]);
			
	refresh();

	saved_min_row = min_row;
	saved_max_row = max_row;
	saved_min_col = min_col;
	saved_max_col = max_col;
	return;
}
