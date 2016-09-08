/*
 *
 *	Library Name:	socketlib
 *		Function: setup_client
 *
 *	Synopsis (Usage and Parameters):
 *
 *	Description:
 *
 *	Programmer:	John C. Noble
 *	Organization:	EP
 *	Host System:	SunOS 4.0.1
 *	Language:	C
 *	Date Created:	02/14/93
 *	Modifications:
 *		$Log$
 */

/* include files */
#include <unistd.h>
#include <strings.h>
#include "socket_utils.h"

/* Program Global Variables */
static int Sockt_client;		/* client socket id */
static int Sockt_server;		/* server socket id */

static int Verbose	= FALSE;

/*  
** setup_client() sets up a client on the client_port, and tries to
** connent to a server on remote_host.
*/
int
setup_client(int client_port_number)
{
	struct hostent *hp;
	struct in_addr my_machine_addr; 	/* inet addr of this machine */
	struct sockaddr_in my_addr;		/* my address */
	struct sockaddr_in his_addr;		/* his address */
	char remote_host[] = "localhost";
	char hostname[HOST_NAME_LEN];		/* name of host machine */
	int length = HOST_NAME_LEN;		
	int cc;				/* character count for send/receive*/
	int i,j;
	int na;
	int connected;			/* connected flag */
	short network_port;		/* port in network byte order	  */
	static int tries = 0;		/* # of connect tries */

	/*
	** get port number
	*/
	network_port = htons (client_port_number);

	if (Verbose)
		fprintf(stderr,"attempting to set up client on %d\n",
							client_port_number);
	/* 
	** Build address structures
	*/
	hp = gethostbyname(remote_host);
	if (hp == (struct hostent *) 0) 
	{
		fprintf(stderr,"Unknown host %s\n", remote_host);
		return(ERROR);
	}
	if (hp->h_addrtype != AF_INET)
	{
		fprintf (stderr, "Wrong protocol on host %s\n", remote_host);
		return(ERROR);
	}
	bzero((char *)&my_addr, sizeof(my_addr));
	bcopy(hp->h_addr, (char *)&my_machine_addr.s_addr, hp->h_length);

	his_addr.sin_family = AF_INET;		
	his_addr.sin_addr = my_machine_addr;
	his_addr.sin_port = network_port;	/* assigned port */


	gethostname(hostname, length);
	hp = gethostbyname(hostname);
	if (hp == (struct hostent *) 0) 
	{
		fprintf(stderr,"Unknown host %s\n", remote_host);
		return(ERROR);
	}
	if (hp->h_addrtype != AF_INET)
	{
		fprintf (stderr, "Wrong protocol on host %s\n", remote_host);
		return(ERROR);
	}
	bzero((char *)&my_addr, sizeof(my_addr));
	bcopy(hp->h_addr, (char *)&my_machine_addr, hp->h_length);

	my_addr.sin_family = AF_INET;		/* internet domain */
	my_addr.sin_addr = my_machine_addr;
	my_addr.sin_port = 0;			/* let UNIX pick a port # */

	/*
	** Create socket, name it, and connect to server.
	** NOTE: a kernel bug makes it necessary for us to close
	** the socket and start again from scratch when a connect fails.
	*/
	if (Verbose)
		fprintf (stderr, "Trying to connect...\n");

	connected = FALSE;
	do {
		if ((Sockt_client = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror ("socket");
			return(ERROR);
		}

		if (connect(Sockt_client, (struct sockaddr *) &his_addr, sizeof(his_addr)) == 0)
			connected = TRUE;
		else
		{
			perror ("connect");
			close(Sockt_client);
			sleep(1);
			if ( tries++ > CONN_ATTEMPTS )
			{
				fprintf (stderr, "exhausted connect attempt limit\n");
				return ERROR;
			}
		}
	} while (!connected);
	if (Verbose)
		fprintf (stderr, "Node Connected on socket %d...\n",
								Sockt_client);
	return (Sockt_client);
}
