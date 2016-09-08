/***************************************************************** 
 * Student: Jason Rice
 * 
 * Course: 605.414 System Development in a Unix Environment
 * 
 * File: log_mgr.c
 *
 * Description:
 *   Contains the implemtations for each function declared with
 * the log manager header files. This library is built with a
 * companion Makefile archive routine. The functions provided
 * allow programs to use this library to set log files, record
 * log events, and close an open log file. All of these operations
 * are atomic and therefore each operation will be an independent
 * single transaction.
 ****************************************************************/
#include "log_mgr.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

// Statics
static int Fd = -1;

// Constants
const int BUFFER_MAX = 128;
const int PREP_MAX   = 256;
const int TIME_MAX    = 50;

int log_event(Levels l, const char *fmt, ...){
	// If a log file has not been set use the default:
	if (Fd == -1) {
		set_logfile(DEFAULT);
		if (Fd == -1) {
         perror("Error setting default log file.");
			return ERROR;
		}
	}

	char buffer[BUFFER_MAX];
	buffer[0] = '\0';

	// First write the ellipsis arguments ('...') with format a buffer.
	va_list args;
	va_start(args, fmt);
	if (vsprintf(buffer,fmt,args) < 0){ 
      perror("Error retrieving arguments.");
		return ERROR;
	}
	va_end(args);

	char prepend[PREP_MAX];
	char str_time[TIME_MAX];
	char str_date[TIME_MAX];

   // Append nulls so string concatenation doesn't get confused.
	prepend[0] = '\0';
	str_time[0] = '\0';
	str_date[0] = '\0';

	struct tm *tm;
	time_t t;
	t = time(NULL);
	tm = localtime(&t);

	strftime(str_date, sizeof(str_date), "%b %d ", tm);
	strftime(str_time, sizeof(str_time), "%X", tm);
	strcat(prepend,str_date);
	strcat(prepend,str_time);

	switch(l){
		case WARNING:
			strcat(prepend, ":WARNING:\0");
			break;
		case FATAL:
		  strcat(prepend, ":FATAL:\0");
			break;
		case INFO:
			strcat(prepend, ":INFO:\0");
			break;
		default:
			strcat(prepend, ":INFO:\0");
	}

	strcat(buffer,"\n");
	strcat(prepend,buffer);

	// Write the output to a log file. First check for errors.
	size_t length = strlen(prepend);
	if (write(Fd,prepend,length) != length){
		perror("Error writing output to log file.");
		return ERROR;
	}
	return OK;
}

int set_logfile(const char *logfile_name){
	if (logfile_name == NULL || logfile_name[0] == '\0'){
		perror("Error setting log file. Log file name is null.");
		return ERROR;
	}

	int tempFd = open(logfile_name, O_WRONLY | O_CREAT | O_TRUNC, 0755);
	// If the log file was successfully opened close the previously opened log file.
	if (tempFd > -1){
		close_logfile();
		// Update the new file descriptor.
		Fd = tempFd;
		return OK;
	}
	// There was a problem opening the log file. Allow the previously open log file to remain open.
	else {
		perror("Error opening the log file. Check permissions etc.");
		return ERROR;
	}
}

void close_logfile()
{  
	close(Fd);
	// reset field descriptor (may already be -1)   
	Fd = -1;
}
