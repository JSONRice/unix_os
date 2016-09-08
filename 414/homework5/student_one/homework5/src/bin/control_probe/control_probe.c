// 
// homework 5, Unix Systems Programming
// Fall 
//
// file: control_probe.c
//
// This program communicates with the probe program and the display 
// program.  This program sends messages over a socket to the probe,
// directing the probe to move around a cavern.  The probe responds
// and indicates whether the move is possible.  This program uses
// the moves and responses to map out the cavern and display the 
// map in shared memory where the display program can show it to the 
// user.  This program must be set to use the same port as the probe
// and the same shared memory key as the display program.
//
// The cave mapping is done with a simple recursive algorithm. Try
// to move in each of the four posible directions.  After each move
// attempt, check if the move is successful, and if it is, recurse.
// This simple algorithm ensures that every contiguous location 
// will be reached.
//
// A move function was written to keep track of the current position
// and update shared memory with the area mapped so far.  This means
// the mapping is done automatically while moving, so the recursive
// exploration does not have to worry about it.
//
// All reading and writing to shared memory and to the socket is done
// through a wrapper.  This consolidates the error checking and makes
// the program more robust.  In the case of reading messages from the
// socket, it also makes it easy to accept an unexpected results msg.
//
// To avoid passing lots of parameters or giving in to lots of globals, 
// a probe_data struct was created.  This one struct has everything 
// needed to keep track of location and to communicate with the probe.
// However, the probe is created as a static global so it is accessable
// for cleanup in the event of an unexpected error.

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "log_mgr.h"
#include "shmlib.h"
#include "messages.h"
#include "socket_utils.h"

#define SHM_ROWS 48
#define SHM_COLS 160

typedef struct _probe_data
{
	int            x;
	int            y;
	int            shmkey;
	char*          shm;
	int            sock;
	PROBE_MESSAGES msg;
} probe_data;

probe_data probe;

void  connect_to_display_server (probe_data* probe, int shmkey);
void  connect_to_probe_server (probe_data* probe, int port);
void  disconnect_from_display_server (probe_data* probe);
void  disconnect_from_probe_sever (int sock);
int   move_probe (probe_data* probe, int direction);
void  explore (probe_data* probe);
void  explode_probe (probe_data* probe);
char  display_read (probe_data* probe, int x, int y);
void  display_write (probe_data* probe, int x, int y, char value);
void  display_bounds_check (int x, int y);
void  init_probe (probe_data* probe, int port, int shmkey, int cavern);
void  free_probe (probe_data* probe);
void  parse_args (int argc, char* argv[], int* shmkey, int* port, int* cavern);
void  usage_exit (void);
void  read_msg (probe_data* probe);
void  write_msg (probe_data* probe);
void  clean_exit (int dummy);

int main (int argc, char* argv[])
{
	int shmkey = -1;
	int port   = 10000;
	int cavern = 0;
	
	parse_args (argc, argv, &shmkey, &port, &cavern);
	init_probe (&probe, port, shmkey, cavern);
	explore (&probe);
	explode_probe (&probe);
	free_probe (&probe);

	return OK;
}

void  parse_args (int argc, char* argv[], int* shmkey, int* port, int* cavern)
{
	int i;
	
	if((argc & 1) == 0)
		usage_exit ();

	for(i=1; i<argc; i=i+2)
	{
		if(argv[i][0] != '-')
			usage_exit ();
		switch(argv[i][1])
		{
			case 'm':
				sscanf (argv[i+1], "%d", shmkey);
				break;
			case 'p':
			case 'P':
				sscanf (argv[i+1], "%d", port);
				break;
			case 'c':
				sscanf (argv[i+1], "%d", cavern);
				break;
		}
	}
	if(*shmkey<0)
		usage_exit ();
	if(*port<0)
		usage_exit ();
	if(*cavern<0)
		usage_exit ();
}

void usage_exit (void)
{
	printf ("Usage: control_probe -m <shared memory key> -p <port number> -c <cavern id>\n");
	printf ("  port number and cavern id are optional.  All numbers should be positive.\n\n");
	exit (ERROR);
}

void init_probe (probe_data* probe, int port, int shmkey, int cavern)
{
	probe->x = 80;
	probe->y = 24;
	connect_to_probe_server (probe, port);  
	connect_to_display_server (probe, shmkey);
	
	//make sure we destroy shm even if exit unexpectedly: 
	signal (SIGTERM, clean_exit); // install handler
	signal (SIGSEGV, clean_exit); // install handler
	
	//insert
	probe->msg.msg_type = MSG_INSERT_PROBE;
	probe->msg.msg.insert.cavern = cavern;
	write (probe->sock, &(probe->msg), sizeof (PROBE_MESSAGES));
}

void  clean_exit (int dummy)
{
	destroy_shm (probe.shmkey);
	exit(ERROR);
}

void  free_probe (probe_data* probe)
{
	disconnect_from_display_server (probe);
	disconnect_from_probe_sever (probe->sock);
}

char display_read (probe_data* probe, int x, int y)
{
	display_bounds_check (x, y);
	return probe->shm[x + SHM_COLS*y];
}

void display_write (probe_data* probe, int x, int y, char value)
{
	display_bounds_check (x, y);
	probe->shm[x + SHM_COLS*y] = value;
}

void display_bounds_check (x, y)
{
	char in_bounds = 1;

	if(x < 0)
		in_bounds = 0;
	if(x >= SHM_COLS)
		in_bounds = 0;
	if(y < 0)
		in_bounds = 0;
	if(y >= SHM_ROWS)
		in_bounds = 0;
	if(!in_bounds)
	{
		printf ("Control Probe accessed display memory out of bounds: x=%d, y=%d.\n", x, y);
		log_event (FATAL, "Control Probe accessed display memory out of bounds: x=%d, y=%d.\n", x, y);
		clean_exit (ERROR);
	}
}

void explore (probe_data* probe) //recursivly check every direction
{
	if(display_read (probe, probe->x, probe->y-1) == 0) //north unexplored
	{
		if(move_probe (probe, MOVE_NORTH))
		{
			explore (probe);
			move_probe (probe, MOVE_SOUTH);
		}
	}
	if(display_read (probe, probe->x+1, probe->y) == 0) //east unexplored
	{
		if(move_probe (probe, MOVE_EAST))
		{
			explore (probe);
			move_probe (probe, MOVE_WEST);
		}
	}
	if(display_read (probe, probe->x, probe->y+1) == 0) //south unexplored
	{
		if(move_probe (probe, MOVE_SOUTH))
		{
			explore (probe);
			move_probe (probe, MOVE_NORTH);
		}
	}
	if(display_read (probe, probe->x-1, probe->y) == 0) //west unexplored
	{
		if(move_probe (probe, MOVE_WEST))
		{
			explore (probe);
			move_probe (probe, MOVE_EAST);
		}
	}
}

void read_msg (probe_data* probe)
{
	int cnt = read (probe->sock, &probe->msg, sizeof (PROBE_MESSAGES));
	if(cnt < sizeof (PROBE_MESSAGES))
	{
		printf ("Error reading message from socket.\n");
		log_event (FATAL, "Error reading message from socket.");
		clean_exit (ERROR);
	}
	
	//must be able to handle results mesage at any time:
	if(probe->msg.msg_type == MSG_RESULTS)
	{
		//display results
		printf ("spaces:   %d\n", probe->msg.msg.results.cavern_size);
		printf ("mapped:   %d\n",  probe->msg.msg.results.amt_mapped);
		printf ("lawsuits: %d\n",  probe->msg.msg.results.lawsuits);
		printf ("moves:    %d\n",  probe->msg.msg.results.moves);
		printf ("move to space ratio: %f\n", (float) probe->msg.msg.results.moves / probe->msg.msg.results.cavern_size);
	
		//log results
		log_event (INFO, "spaces:   %d\n", probe->msg.msg.results.cavern_size);
		log_event (INFO, "mapped:   %d\n",  probe->msg.msg.results.amt_mapped);
		log_event (INFO, "lawsuits: %d\n",  probe->msg.msg.results.lawsuits);
		log_event (INFO, "moves:    %d\n",  probe->msg.msg.results.moves);
		log_event (INFO, "move to space ratio: %f\n", (float) probe->msg.msg.results.moves / probe->msg.msg.results.cavern_size);
	}
}

void write_msg (probe_data* probe)
{
	int cnt = write (probe->sock, &probe->msg, sizeof (PROBE_MESSAGES));
	if(cnt < sizeof (PROBE_MESSAGES))
	{
		printf ("Error writing message to socket.\n");
		log_event (FATAL, "Error writing message to socket.");
		clean_exit (ERROR);
	}
}

void explode_probe (probe_data* probe)
{
	//send explode message
	probe->msg.msg_type = MSG_EXPLODE;
	write_msg (probe);
	
	//read response
	read_msg (probe);
	if(probe->msg.msg_type != MSG_RESULTS)
	{
		printf ("Unexpected response from probe after exploding.\n");
		log_event (FATAL, "Unexpected response from probe after exploding");
		clean_exit (ERROR);
	}
}

int move_probe (probe_data* probe, int direction)
{
	int success;
	
	//move
	probe->msg.msg_type = MSG_MOVE;
	probe->msg.msg.move.move = direction;
	write_msg (probe);
	
	//read response
	read_msg (probe);
	if(probe->msg.msg_type != MSG_MV_RESPONSE)
	{
		printf ("Unexpected response from probe after moving");
		log_event (FATAL, "Unexpected response from probe after moving");
		clean_exit (ERROR);
	}
	success = probe->msg.msg.mv_response.success;
	
	if(success)
	{
		//mark current location as explored
		display_write (probe, probe->x, probe->y, '.');
	
		switch(direction)
		{
			case MOVE_NORTH:
				probe->y--;
				break;
			case MOVE_SOUTH:
				probe->y++;
				break;
			case MOVE_EAST:
				probe->x++;
				break;
			case MOVE_WEST:
				probe->x--;
				break;
		}
		
		//mark new location
		display_write (probe, probe->x, probe->y, '@');
	}
	else //move failed, wall must be there
	{
		switch(direction)
		{
			case MOVE_NORTH:
				display_write (probe, probe->x, probe->y-1, '#');
				break;
			case MOVE_SOUTH:
				display_write (probe, probe->x, probe->y+1, '#');
				break;
			case MOVE_EAST:
				display_write (probe, probe->x+1, probe->y, '#');
				break;
			case MOVE_WEST:
				display_write (probe, probe->x-1, probe->y, '#');
				break;
		}
	}
	return success;
}

void connect_to_display_server (probe_data* probe, int shmkey)
{
	log_event (INFO, "connecting to shared memory using key: %d", shmkey);

	probe->shmkey = shmkey;
	probe->shm = (char*) connect_shm (shmkey, SHM_ROWS*SHM_COLS);
	if(probe->shm == NULL)
	{
		printf ("Unable to connect to shared memory\n");
		log_event (FATAL, "Unable to connect to shared memory");
		exit (ERROR);
	}
	memset (probe->shm, 0,  SHM_ROWS*SHM_COLS); //not needed if memory created
}

void disconnect_from_display_server (probe_data* probe)
{
	log_event (INFO, "disconnecting from shared memory.");
	
	display_write (probe, probe->x, probe->y, ')'); // done signal
	if(destroy_shm (probe->shmkey) != OK)
	{
		printf ("Unable to destroy shared memory\n");
		log_event (FATAL, "Unable to disconnect from shared memory");
		exit (ERROR);
	}
}

void connect_to_probe_server (probe_data* probe, int port)
{
	log_event (INFO, "connecting to socket using port: %d", port);
	
	probe->sock = setup_client (port);
	if(probe->sock < 0)
	{
		printf ("Unable to connect to probe server on port %d.  Is it running?\n", port);
		log_event (FATAL, "Unable to connect to probe server on port %d.  Is it running?", port);
		exit (ERROR);
	} 
}

void disconnect_from_probe_sever (int sock)
{
	log_event (INFO, "disconnecting from socket.");
	
	if(close (sock) != 0)
	{
		printf ("Unable to disconnect from probe server\n");
		log_event (FATAL, "Unable to disconnect from probe server");
		exit (ERROR);
	}
}
