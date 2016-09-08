/*
 * Name: Student 2
 * Proj: HW #2
 * Date: 
 * Desc: Library that provides mechanism to write log entries, change
 *       log file and closing log file
*/

#include "log_mgr.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <sys/file.h>
#include <sys/fcntl.h>
#include <unistd.h>

int logfile_fd_g = -1;

int log_event (Levels l, const char *fmt, ...)
{
	char message[MAXLENGTH], timeStr[MINLENGTH], buffer[MAXLENGTH];
	char level[MINLENGTH];
	va_list ap;
	int status;
	time_t currTime;
	int size;
	int fd;

	/* getting the value from the ... argument */
	va_start(ap, fmt);	
	vsprintf(message, fmt, ap);
	va_end(ap);
      
	status = convert_level_to_text(level, l);
	currTime = time(NULL);
	size = strftime(timeStr, sizeof(timeStr), "%b %d %H:%M:%S %Z %Y", localtime(&currTime));

	if (size == 0) {
		return (ERROR);
	}

	/* check for file descriptor */
	if (logfile_fd_g < 0) 
	{
		status = set_logfile(LOGFILE);
                if (status == ERROR) 
			return (ERROR);
	}
  	sprintf(buffer, "%s:%s:%s\n", timeStr, level, message);
	status = write(logfile_fd_g, buffer, strlen(buffer));

	if (status < 0) {
		return (ERROR);
	}

	return (OK);
}

int convert_level_to_text (char *textValue, Levels l) 
{
	int status = OK;

	if (l == 0) {
		strcpy(textValue, "INFO");
	}
	else if (l == 1) {
		strcpy(textValue, "WARNING");
	}
	else if (l == 2) {
		strcpy(textValue, "FATAL");
	}
	else {
		status = ERROR;
	}

	return status;
}

int set_logfile (const char *logfile_name)
{
	int fd;
	int status = OK;

	fd = open(logfile_name, O_CREAT | O_WRONLY | O_APPEND, 0600);
	
	if (fd < 0)
	{
		printf("Unable to open or create %s", logfile_name);
	
		return (ERROR);
	}

	if (logfile_fd_g > 0) 
		status = close(logfile_fd_g);
	logfile_fd_g = fd;

	return (status);
}

void close_logfile(void) 
{
	close(logfile_fd_g);
	logfile_fd_g = -1;
}
