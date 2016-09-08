/*
 *
 *	Library Name:	socketlib
 *		Function:	setup_server
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

static int Verbose	= TRUE;

/* 
** setup_server() sets up a server on the server_port, and waits for
** a client to connect to it.
*/

int
setup_server(int server_port)
{
	short network_port;		/* port in network byte order */
	int i,j;
	int na;
	int length = sizeof (struct sockaddr_in);		
	struct sockaddr_in my_addr;	/* my mailbox */
	struct sockaddr_in his_addr;	/* hold information from */
					/* returned from the accept call */

	network_port = htons (server_port);
	/*
	** Initialize address structure for server's port
	*/
	bzero(&my_addr, sizeof(struct sockaddr_in));

	if (Verbose)
		fprintf (stderr, "attempting to set up server on %d\n", server_port);
	
	bzero((char *)&my_addr, sizeof(struct sockaddr_in));

	my_addr.sin_family = AF_INET;		/* internet domain */
	my_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	my_addr.sin_port = network_port;

	/* obtain and bind socket to accept connections on */
	if ((na = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror ("socket");
		return(ERROR);
	}
	if ( bind(na, (struct sockaddr *) &my_addr, sizeof(my_addr)) != 0) 
	{
		perror ("bind");
		return(ERROR);
	}
	
	listen(na, 0);	/* allow 0 connections to be queued */
	if (Verbose)
		fprintf (stderr, "server listening...\n");

        if ((Sockt_server = accept(na, (struct sockaddr *) &his_addr, &length )) < 0) 
	{
		perror ("accept");
		close(Sockt_server);
		return(ERROR);
	}
	close (na);
	return(Sockt_server);
}
