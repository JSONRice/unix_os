/* include files */
# include <errno.h>
# include <stdio.h>
# include <sys/file.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netdb.h>

/* Program global Defines */
#define SHUTDOWN	1
#define NO_SHUTDOWN	0
#define SERVER_FIRST	1
#define CLIENT_FIRST	0
#define HOST_NAME_LEN	256
#define ERROR		-1
#define TRUE		1
#define FALSE		0
#define	UNINITIALIZED   0
#define	CONN_ATTEMPTS	100
#define OK		0

/*
 * Function prototypes
 */

int
setup_server (int port);

int
setup_client (int port);
