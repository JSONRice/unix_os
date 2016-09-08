# include <stdio.h>
# include <stdarg.h>
# include <errno.h>
# include <string.h>

#define OK	0
#define ERROR	(-1)

/* Constants */
# define MAXNAME	256
# define LOGDIR		"log"

/* log file name and FILE buffers */
static char Logname [MAXNAME];
static FILE *Logfile = NULL;

void
logmsg(char *message, ...);

int
init_reality(char *filename)
{
	if (open_reality_log(filename) == ERROR)
		return ERROR;

	return OK;
}

int
open_reality_log(char *filename)
{
	long t, time();
	char *ts, *ctime();

	sprintf(Logname, "%s/%s.log", LOGDIR, filename);

	Logfile = fopen( Logname, "w");
	if (Logfile == NULL) {
		fprintf(stderr, "Can't open %s", Logname);
		return ERROR;
	}

	logmsg("<<< Log File: %s >>>", Logname);

	t = time(0);
	ts = ctime(&t);
	ts[ strlen(ts)-6 ] = 0;
	logmsg("<<< Startup Date-Time = %s >>>", ts );

	return OK;
}


/*
** This is a poor-man's way of implementing a function with a
** variable number of parameters.
*/
void
logmsg(char *message, ...)
{
	long tval, time();
	char tbuf[32], *ctime();
	FILE *lfile;
	va_list	argp;

	if (Logfile != NULL)
		lfile = Logfile;
	else 	lfile = stderr;

	while (*message == '\n') {
		putc('\n', lfile);
		++message;
	}
	if (*message == '\0') {
		putc('\n', lfile);
		fflush(lfile);
		return;
	}

	va_start (argp, message);
	tval = time(0);
	fprintf(lfile, "%d> ", tval);

	vfprintf(lfile, message, argp);
	putc('\n', lfile);
	fflush(lfile);
}


/*
** This is a poor-man's way of implementing a function with a
** variable number of parameters.
*/
void
logerr(char *message, ...)
{
	long tval, time();
	char tbuf[32], *ctime();
	FILE *lfile;
	va_list argp;

	if (Logfile != NULL)
		lfile = Logfile;
	else 	lfile = stderr;

	while (*message == '\n') {
		putc('\n', lfile);
		++message;
	}
	if (*message == '\0') {
		putc('\n', lfile);
		fflush(lfile);
		return;
	}

	tval = time(0);
	fprintf(lfile, "%d> ", tval);

	fputs("ERROR: ", lfile);
	vfprintf(lfile, message, argp);
	putc('\n', lfile);
	fflush(lfile);
}

/*
** This is a poor-man's way of implementing a function with a
** variable number of parameters.
*/
void
logsyserr(char *message, ...)
{
	long tval, time();
	char tbuf[32], *ctime();
	FILE *lfile;
	va_list argp;

	if (Logfile != NULL)
		lfile = Logfile;
	else 	lfile = stderr;

	while (*message == '\n') {
		putc('\n', lfile);
		++message;
	}
	if (*message == '\0') {
		putc('\n', lfile);
		fflush(lfile);
		return;
	}

	tval = time(0);
	fprintf(lfile, "%d> (%d - %s)", tval, errno, strerror (errno));

	fputs("ERROR: ", lfile);
	vfprintf(lfile, message, argp);
	putc('\n', lfile);
	fflush(lfile);
}

