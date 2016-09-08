#include <sys/types.h>
#include <stdio.h>
#include "messages.h"
#include "reality.h"

#define MAX_COLUMN		80
#define MAX_ROW			23

#define SPACE			' '
#define WALL			'#'
/*
#define WALL			'\030'
*/

#define MAX_CAVITYS		(MAX_ROW*MAX_COLUMN)

int PRow, PCol;			/* player's current position */

static struct {
	char sym;
	char map;
} Maze[MAX_ROW][MAX_COLUMN];

struct cmap {
	int row, col;
	char linked;
};
int Num_Moves;

/**************************************************/
/**************************************************/
fill_cavity(row, col)
int row, col;
{
	Maze[row][col].sym = WALL;

	if (Maze[row+1][col].sym == SPACE)
		fill_cavity(row+1, col);
	if (Maze[row-1][col].sym == SPACE)
		fill_cavity(row-1, col);
	if (Maze[row][col+1].sym == SPACE)
		fill_cavity(row, col+1);
	if (Maze[row][col-1].sym == SPACE)
		fill_cavity(row, col-1);
}

/**************************************************/
/**************************************************/
map_cavity(row, col)
int row, col;
{
	int num = 1;

	Maze[row][col].map = TRUE;

	if (Maze[row+1][col].sym == SPACE && Maze[row+1][col].map == FALSE)
		num += map_cavity(row+1, col);
	if (Maze[row-1][col].sym == SPACE && Maze[row-1][col].map == FALSE)
		num += map_cavity(row-1, col);
	if (Maze[row][col+1].sym == SPACE && Maze[row][col+1].map == FALSE)
		num += map_cavity(row, col+1);
	if (Maze[row][col-1].sym == SPACE && Maze[row][col-1].map == FALSE)
		num += map_cavity(row, col-1);

	return num;
}

/**************************************************/
/*
** Connect a subset of cavitys.  Some others will randomly
** be connected during this process
*/
/**************************************************/
connect_cavitys(cavity_map, num_cavitys)
struct cmap *cavity_map;
int num_cavitys;
{
	int curi, nexti;
	int i;
	int num;


	if (num_cavitys > 5)
		num = 5;
	else
		num = num_cavitys;

	/*
	printf("Connecting %d cavitys\n", num);
	*/

	curi = rand() % num_cavitys;
	cavity_map[curi].linked = TRUE;

	i = 1;
	while (i < num)
	{
		do {
			nexti = rand() % num_cavitys;
		} while (cavity_map[nexti].linked == TRUE);

		/*
		printf("connecting cavity %d to %d\n", curi, nexti);
		*/

		bore_tunnel(&cavity_map[curi], &cavity_map[nexti]);

		cavity_map[nexti].linked = TRUE;

		curi = nexti;
		i++;
	}
}

/**************************************************/
/**************************************************/
bore_tunnel(curcav, nextcav)
struct cmap *curcav, *nextcav;
{
	/*
	printf("boring hole from %d %d to %d %d\n",
				curcav->row, curcav->col,
				nextcav->row, nextcav->col);
	*/

	while ( (curcav->row != nextcav->row) ||
	        (curcav->col != nextcav->col) )
	{
		if ( randn(50) )
		{
			if (nextcav->row > curcav->row)
				curcav->row++;
			else if (nextcav->row < curcav->row)
				curcav->row--;
		}
		else
		{
			if (nextcav->col > curcav->col)
				curcav->col++;
			else if (nextcav->col < curcav->col)
				curcav->col--;
		}
		Maze[curcav->row][curcav->col].sym = SPACE;
		/*
		printf("bored %d %d\n", curcav->row, curcav->col);
		*/
	}
}

/**************************************************/
/**************************************************/
create_maze(seed)
int seed;
{
	int row, col;
	int total_mapped;
	int size;
	int num_cavitys;
	struct cmap cavity_map[MAX_CAVITYS];

	seed_maze(seed);

	do {
		/*
		** unmap the maze
		*/
		for (row=1; row<MAX_ROW-1; row++)
			for (col=1; col < MAX_COLUMN-1; col++)
				Maze[row][col].map = FALSE;

		/*
		** map the cavitys
		*/
		num_cavitys = 0;
		for (row=1; row<MAX_ROW-1; row++)
			for (col=1; col < MAX_COLUMN-1; col++)
				if (Maze[row][col].sym == SPACE &&
				    Maze[row][col].map == FALSE)
				{
					if (map_cavity(row, col) > 2)
					{
						cavity_map[num_cavitys].row = row;
						cavity_map[num_cavitys].col = col;
						cavity_map[num_cavitys].linked = FALSE;

						num_cavitys++;
					}
					else
						fill_cavity(row, col);
				}

		connect_cavitys(cavity_map, num_cavitys);

	} while (num_cavitys > 1);

	/*
	** unmap the maze; player will remap it
	*/
	for (row=1; row<MAX_ROW-1; row++)
		for (col=1; col < MAX_COLUMN-1; col++)
			Maze[row][col].map = FALSE;

	/*
	** Place player at a random location
	*/
	do {
		PRow = (rand() % (MAX_ROW-2)) + 1;
		PCol = (rand() % (MAX_COLUMN-2)) + 1;
	} while (Maze[PRow][PCol].sym != SPACE);

	Maze[PRow][PCol].map = TRUE;

}

/**************************************************/
/**************************************************/
count_mapped()
{
	int num = 0;
	int row, col;

	for (row=1; row<MAX_ROW-1; row++)
		for (col=1; col < MAX_COLUMN-1; col++)
			if (Maze[row][col].map == TRUE)
					num++;
	return num;
}

/**************************************************/
/**************************************************/
get_maze_results(results)
struct msg_generic *results;
{
	int row, col;
	int missed;

	results->msg_type = MSG_RESULTS;
	results->msg.results.cavern_size = 0;
	results->msg.results.amt_mapped = 0;
	results->msg.results.moves = Num_Moves;

	for (row=1; row<MAX_ROW-1; row++)
		for (col=1; col < MAX_COLUMN-1; col++)
			if (Maze[row][col].sym == SPACE)
			{
				results->msg.results.cavern_size++;
				if (Maze[row][col].map == TRUE)
					results->msg.results.amt_mapped++;
			}
	if (results->msg.results.amt_mapped != results->msg.results.cavern_size)
	{
		missed = results->msg.results.cavern_size - 
					results->msg.results.amt_mapped; 
		if (missed < 10)
			results->msg.results.lawsuits = 1; 
		else
			results->msg.results.lawsuits = (missed % 10) + 1; 

	}
		
}

	
/**************************************************/
/**************************************************/
seed_maze(seed)
int seed;
{
	int row, col;

	if (seed)
		srand(seed);
	else
		srand(getpid());

	/*
	** All outside boundaries and randomly located inside cells
	** are walls.  The remainder are spaces.
	*/
	for (row=0; row<MAX_ROW; row++)
		for (col=0; col < MAX_COLUMN; col++)
		{
			if (row == 0 || row == MAX_ROW-1 ||
			    col == 0 || col == MAX_COLUMN-1 ||
			    randn(70) )
			{
				Maze[row][col].sym = WALL;
			}
			else
				Maze[row][col].sym = SPACE;
		}

}

/******************************************************************/
/*
** num is a number between 1 and 99 representing % probability
*/
/******************************************************************/
randn(num)
int num;
{
	if (num < 1)
		return FALSE;
	if (num > 99)
		return FALSE;

	if ( (rand() % 100) < num)
		return TRUE;
	else
		return FALSE;
}

/**************************************************/
/**************************************************/
print_maze(str)
char *str;
{
	int row, col;

	printf(str);

	for (row=0; row < MAX_ROW; row++)
	{
		for (col=0; col<MAX_COLUMN; col++)
		{
			if (PRow == row && PCol == col)
				putchar('X');
			else if (Maze[row][col].sym == WALL)
				putchar(Maze[row][col].sym);
			else
			{
				if (Maze[row][col].map == TRUE)
					putchar('+');
				else
					putchar(' ');
			}
		}
		putchar('\n');
	}
	putchar('\n');
}

/**************************************************/
/**************************************************/
move_player(message, response)
struct msg_generic *message;
struct msg_generic *response;
{
	int nrow, ncol;

	response->msg_type = MSG_MV_RESPONSE;

	/*
	logmsg("player moved %s",
		msg_move->move == MOVE_NORTH ? "N" :
		(msg_move->move == MOVE_SOUTH ? "S" :
		(msg_move->move == MOVE_EAST ? "E" : "W")) );  
	*/

	Num_Moves++;
	/*
	if ( (++Num_Moves % 100) == 0)
		logmsg("%d moves so far", Num_Moves);
	*/


	switch (message->msg.move.move)
	{
	  case MOVE_NORTH:
		nrow = PRow - 1;
		ncol = PCol;

		if (Maze[nrow][ncol].sym != WALL)
		{
			response->msg.mv_response.success = YES;
			PRow = nrow;
			PCol = ncol;
			Maze[PRow][PCol].map = TRUE;
		}
		else
			response->msg.mv_response.success = NO;
		break;
	  case MOVE_SOUTH:
		nrow = PRow + 1;
		ncol = PCol;

		if (Maze[nrow][ncol].sym != WALL)
		{
			response->msg.mv_response.success = YES;
			PRow = nrow;
			PCol = ncol;
			Maze[PRow][PCol].map = TRUE;
		}
		else
			response->msg.mv_response.success = NO;
		break;
	  case MOVE_EAST:
		nrow = PRow;
		ncol = PCol + 1;

		if (Maze[nrow][ncol].sym != WALL)
		{
			response->msg.mv_response.success = YES;
			PRow = nrow;
			PCol = ncol;
			Maze[PRow][PCol].map = TRUE;
		}
		else
			response->msg.mv_response.success = NO;
		break;
	  case MOVE_WEST:
		nrow = PRow;
		ncol = PCol - 1;

		if (Maze[nrow][ncol].sym != WALL)
		{
			response->msg.mv_response.success = YES;
			PRow = nrow;
			PCol = ncol;
			Maze[PRow][PCol].map = TRUE;
		}
		else
			response->msg.mv_response.success = NO;
		break;
	  default:
		response->msg.mv_response.success = NO;
	}

	/*
	logmsg("%ssuccessful move (%s) to %d %d", 
			response->msg.mv_response.success == YES ? "" : "UN", 
			message->msg.move.move == MOVE_NORTH ? "N" :
			  (message->msg.move.move == MOVE_SOUTH ? "S" :
			  (message->msg.move.move == MOVE_EAST ? "E" : "W")),
			PRow, PCol);
	*/
}
