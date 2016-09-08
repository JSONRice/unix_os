#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include "messages.h"
#include "reality.h"


static int MyPid;

play_game(fd)
int fd;
{
	fd_set 	readfds;
	struct msg_generic message;
	int die();
	int timeout();

	/*
	** Parent simply closes fd and returns
	*/
	if (fork() != 0)
	{
		close(fd);
		return;
	}

	MyPid = getpid();
	signal(SIGPIPE, die);
	signal(SIGALRM, timeout);

	alarm(THIRTY_MINUTES);

	/*
	** I am the child, so I must create a maze and let the client
	** explore it
	*/
	/*
	close(0);
	close(1);
	close(2);
	*/

	do { 
		FD_ZERO(&readfds);
		FD_SET(fd, &readfds);

		select(fd+1, &readfds, 0, 0, 0);

		if (get_message(fd, &message) != ERROR)
				respond_to_message(fd, &message);

	} while ( FD_ISSET(fd, &readfds) );
}

/**************************************************/
/**************************************************/
get_message(fd, message)
int fd;
struct msg_generic *message;
{
	int running_total = 0;
	int to_read;
	int num_read;
	int nbytes;


	nbytes = sizeof (struct msg_generic);
	to_read = nbytes;
	running_total = 0;

	do {
		num_read = read(fd, message, to_read);
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
respond_to_message(fd, message)
int fd;
struct msg_generic *message;
{
	static char maze_created = FALSE;
	struct msg_generic response;
	extern int Num_Moves;


	if (!maze_created && message->msg_type != MSG_INSERT_PROBE)
		return;

	switch (message->msg_type)
	{
	  case MSG_DISCONNECT:
		/*
		logmsg("%d: Got DISCONNECT msg, exiting...", MyPid);  
		*/
		die();
		break;
	  case MSG_INSERT_PROBE:
		/*
		logmsg("%d: Creating cavern with seed = %d", MyPid, message->msg.insert.cavern);  
		*/
		create_maze(message->msg.insert.cavern);
		maze_created = TRUE;
		break;
	  case MSG_PRINT_MAZE:
		if (getuid() == 158)
		{
			/*
			logmsg("%d: Printing maze", MyPid);  
			*/
			print_maze("player ordered\n");
		}
		else
			/*
			logmsg("%d: Not authorized for maze print", MyPid);
			*/
		break;
	  case MSG_EXPLODE:
		get_maze_results(&response);

		/*
		logmsg("%d: %d mapped out of %d", MyPid, 
			response.msg.results.amt_mapped,
			response.msg.results.cavern_size);
		*/

		send_to_player(fd, &response);

		die("Exploded bomb");
		/*
		print_maze("after results\n");
		*/

		break;
	  case MSG_MOVE:
		move_player(message, &response);

		send_to_player(fd, &response);

		if (Num_Moves > 5000)
		{
			get_maze_results(&response);
			send_to_player(fd, &response);
			die("Exceeded 5000 moves");
		}
		break;
	  default:
		/*
		logmsg("%d: Unknown message: %d", MyPid, message->msg_type);
		*/
		break;
	}
}


/**************************************************/
/**************************************************/
send_to_player(fd, message)
int fd;
struct msg_generic *message;
{
	int             n;
	int             nwritten;
	char           *inet_ntoa();
	extern int 	errno;
	int		nbytes;
	static int	counter;

	if (fd < 0)
	{
		/*
		logmsg("%d: Can not write to player, exiting...", MyPid);
		*/
		die();
	}

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
				/*
				logmsg("%d: Lost socket, exiting...", MyPid);
				*/
				die();
			}
			/*
			logmsg("%d: sleeping", MyPid);
			*/
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
die(str)
char *str;
{
	/*
	logmsg("%d: dying because %s", MyPid, str);
	*/
	exit(0);
}

/**************************************************/
/**************************************************/
timeout()
{
	/*
	logmsg("%d: 30 minute game, quitting", MyPid);
	*/
	exit(0);
}
