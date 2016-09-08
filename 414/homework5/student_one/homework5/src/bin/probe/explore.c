#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include "probe.h"
#include "messages.h"

static int MyPid;
#define MAX_ALLOWED_MOVES	10000
void respond_to_message();

void
die(char *);

void
timeout (int);

void
send_to_player(int, PROBE_MESSAGES *);

int
play_game(int fd)
{
	fd_set 	readfds;
	struct msg_generic message;
	void lost_connection(int sig);

	/*
	** Parent simply closes fd and returns
	*/
	if (fork() != 0)
	{
		close(fd);
		return;
	}

	MyPid = getpid();
	signal(SIGPIPE, lost_connection);
	signal(SIGALRM, timeout);

	alarm(THIRTY_MINUTES);

	/*
	** I am the child, so I must create a maze and let the client
	** explore it
	*/

	do { 
		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);

		select(fd+1, &readfds, 0, 0, 0);

		if (get_message(fd, &message) != ERROR)
				respond_to_message(fd, &message);

	} while ( FD_ISSET(fd, &readfds) );
}

void
lost_connection(int sig)
{
	die ("Broken pipe");
	return;
}

/**************************************************/
/**************************************************/
get_message(int fd, PROBE_MESSAGES *message)
{
	int running_total = 0;
	int to_read;
	int num_read;
	int nbytes;


	nbytes = sizeof (PROBE_MESSAGES);
	to_read = nbytes;
	running_total = 0;

	do {
		num_read = read(fd, &(message[running_total]), to_read);
		if (num_read <= 0)
		{
			sleep(1);
		}
		else
			running_total += num_read;

		to_read = nbytes - running_total;

	} while (running_total < nbytes);

	return OK;
}

/**************************************************/
/**************************************************/
void
respond_to_message(int fd, PROBE_MESSAGES *message)
{
	static char maze_created = FALSE;
	PROBE_MESSAGES response;
	extern int Num_Moves;
	extern int is_priv_user();


	if (!maze_created && message->msg_type != MSG_INSERT_PROBE)
	{
		if (debugging())
			logmsg("%d: Got msg_type %d before probe inserted - ignored.", MyPid, message->msg_type);
			
		return;
	}
	if (debugging())
		logmsg ("%d: Processing msg_type %d from client", getpid(), message->msg_type);

	switch (message->msg_type)
	{
	  case MSG_DISCONNECT:
		if (debugging())
			logmsg("%d: Got DISCONNECT msg, exiting...", MyPid);  
		die("disconnecting");
		break;
	  case MSG_INSERT_PROBE:
		if (debugging())
			logmsg("%d: Creating cavern with seed = %d", MyPid, message->msg.insert.cavern);  
		create_maze(message->msg.insert.cavern);
		maze_created = TRUE;
		break;
	  case MSG_PRINT_MAZE:
		if (is_priv_user())
		{
			if (debugging())
				logmsg("%d: Printing maze", MyPid);  
			print_maze("player ordered\n");
		}
		else
			if (debugging())
				logmsg("%d: Not authorized for maze print", MyPid);
		break;
	  case MSG_EXPLODE:
		get_maze_results(&response);

		if (debugging())
			logmsg("%d: %d mapped out of %d", MyPid, 
				response.msg.results.amt_mapped,
				response.msg.results.cavern_size);

		send_to_player(fd, &response);

		die("Exploded bomb");
		if (debugging())
			print_maze("after results\n");

		break;
	  case MSG_MOVE:
		move_player(message, &response);

		send_to_player(fd, &response);

		if (Num_Moves > MAX_ALLOWED_MOVES)
		{
			printf ("Exceeded move count\n");
			get_maze_results(&response);
			send_to_player(fd, &response);
			die("Exceeded 5000 moves");
		}
		break;
	  default:
		if (debugging())
			logmsg("%d: Unknown message: %d", MyPid, message->msg_type);
		break;
	}
}


/**************************************************/
/**************************************************/
void
send_to_player(int fd, PROBE_MESSAGES *message)
{
	int             n;
	int             nwritten;
	char           *inet_ntoa();
	extern int 	errno;
	int		nbytes;
	static int	counter;

	if (fd < 0)
	{
		if (debugging())
			logmsg("%d: Can not write to player, exiting...", MyPid);
		die("bad fd");
	}

	if (debugging())
		logmsg ("%d: Sending msg_type %d to client", getpid(), message->msg_type);
	if( (counter++ % 5) == 0 )
		sleepms(25);

	nbytes = sizeof (struct msg_generic);
	nwritten = 0;
	while (nwritten < nbytes)
	{
		n = write(fd, message + nwritten, nbytes - nwritten); 
		if (n < 0) 
		{
			if (errno == EPIPE) 
			{
				close(fd);
				if (debugging ())
					logmsg("%d: Lost socket, exiting...", MyPid);
				die("Lost socket");
			}
			if (debugging())
				logmsg("%d: sleeping", MyPid);
			sleep(1);
		}
		else
			nwritten += n;
	}
	if (nwritten != nbytes)
		logerr("send_to_player: wrote %d bytes of %d byte buffer",
			nwritten, nbytes);

}

/**************************************************/
/**************************************************/
void
die(char *str)
{
	if (debugging())
		logmsg("%d: dying because %s", MyPid, str);
	exit(0);
}

/**************************************************/
/**************************************************/
void
timeout(int sig)
{
	if (debugging())
		logmsg("%d: 30 minute game, quitting", MyPid);
	exit(0);
}
