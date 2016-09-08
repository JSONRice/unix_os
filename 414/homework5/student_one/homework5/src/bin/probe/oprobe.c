
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <ctype.h>
#include "reality.h"
#include "messages.h"

#define TWO_SECONDS	2
#define INFINITE_LOOP	69

/*
 * Sock is socket through which all connections from clients are
 * made. When a connection is made, the communicating socket is
 * kept in the fd field of the conn structure.
 */
static int      Sock;

main(argc, argv)
int argc;
char **argv;
{
	int 	fd;
	struct sockaddr_in from;
	int 	len = sizeof from;
	int reap_child();
	int bored();
	int probe_port = PROBE_PORT;
	extern int      opterr;	/* used by getopt(3) */
	extern char    *optarg;	/* used by getopt(3) */
	char opt;


	/*
	init_reality("cavern");
	*/

	opterr = 0;		/* suppress getopt(3) generated messages */
	while ((opt = getopt(argc, argv, "P:")) != EOF)
		switch (opt) 
		{
		  case 'P':
			probe_port = atoi(optarg);
			break;
		  default:
			fprintf(stderr, "Illegal option %c\n", opt); 
			break;
		}


	signal(SIGCHLD, reap_child);
	signal(SIGALRM, bored);

	while (establish_socket(probe_port) == ERROR) {
		logerr("establish socket failed - waiting 5 secs.");
		sleep(5);
	}

	/*
	logmsg("Waiting for connections");
	*/
	while(INFINITE_LOOP)
	{
		alarm(THIRTY_MINUTES);
		fd = accept(Sock, (struct sockaddr *) (&from), &len);
		alarm(0);

		if (fd > 0) 
		{
			/*
			logmsg("Accepted connection for fd = %d", fd);
			*/
			play_game(fd);
		}

		sleep(1);
	}
}


/**************************************************/
/**************************************************/
establish_socket(probe_port)
int probe_port;
{
	struct sockaddr_in server;
	int             flags;

	Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (Sock < 0) {
		logsyserr("opening stream socket");
		return ERROR;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(probe_port);
	if (bind(Sock, (struct sockaddr *) (&server), sizeof server) < 0) {
		logsyserr("binding stream socket");
		(void) close(Sock);
		return ERROR;
	}


	if (listen(Sock, 5) < 0) {
		logsyserr("listen");
		(void) close(Sock);
		return ERROR;
	}

	return OK;
}

/**************************************************/
/*
** Reap any dead children
*/
/**************************************************/
reap_child()
{
	union wait status;

	wait3(&status, WNOHANG, 0);
}

/*************************************************************/
/*
** I'm bored.  No one is connecting with me.  I'm gonna quit.
*/
/*************************************************************/
bored()
{
	die("Boredom");
}
