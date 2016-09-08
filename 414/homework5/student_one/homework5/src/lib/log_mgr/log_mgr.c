// 
// homework 3, Unix Systems Programming
// Fall 
//
// file: log_mgr.c
// This file gets built as liblog_mgr.a
// log manager is a utility to write messages to a log file.  Messages
// are tagged with the date and a severity level.  The date is formated
// as follows: day of week, month, day of month, HH:MM:SS, year, time zone.
// After the date, the level is printed: Info, Warning, or Fatal.  Finally
// the log message is printed.  A new line is always printed last.  
// If the last character of the message is a newline, it will be stripped
// to prevent an unnecessary double new line.  For a description of the
// public functions, see the header file, log_mgr.h

#include <sys/file.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "log_mgr.h"

static const char* INFO_STR        = "Info   ";
static const char* WARNING_STR     = "Warning";
static const char* FATAL_STR       = "Fatal  ";

static int Fd = -1;    //file descriptor for the logfile

//time format:
//day of week, month, day of month, HH:MM:SS, year, time zone
//
#define DATE_TIME_FORMAT  "%a %b %d %k:%M:%S %G %Z" 

//set reasonable maximum lengths for the strings:
#define MAX_TIME_STR_LEN  40
#define MAX_USER_STR_LEN  256
#define MAX_TOTAL_LEN     MAX_TIME_STR_LEN + MAX_USER_STR_LEN + 9

// debugging support:
// instead of using printf for debugging, i wrote a wrapper
// function for printf called debug.  this lets me turn on or
// turn off debug printing easily. simply uncomment the define.
//
void debug ( const char *fmt, ... );
//#define DEBUG_ON

int log_event ( Levels l, const char *fmt, ... )
{
	va_list     ap;
	char        dt_str[MAX_TIME_STR_LEN];
	const char* level_str;
	char        user_str[MAX_USER_STR_LEN];
	char        log_str[MAX_TOTAL_LEN];
	time_t      the_time;
	struct tm   time_struct;
	int         open_result;
	
	va_start(ap, fmt); //init the vararg list
	
	//check if file is valid	
	if (Fd < 0)
	{
		//file not valid, open default
		open_result = set_logfile(DEFAULT_LOGFILE);
		if(open_result != OK)
			return ERROR; //could not open!
	}
	
	//build the date time string
	the_time = time(NULL);
	localtime_r(&the_time, &time_struct);
	strftime(dt_str, MAX_TIME_STR_LEN, DATE_TIME_FORMAT, &time_struct);
	
	//build the level string
	switch(l)
	{
		case(INFO):
			level_str = INFO_STR;
			break;
		case(WARNING):
			level_str = WARNING_STR;
			break;
		default:   //(FATAL):
			level_str = FATAL_STR;
			break;
	}
		
	//build user_string
	vsnprintf(user_str, MAX_USER_STR_LEN, fmt, ap);
	if(user_str[strlen(user_str)-1] == '\n')   //strip off trailing newline
		user_str[strlen(user_str)-1] = '\0';
	
	//build combined string for log entry
	snprintf(log_str, MAX_TOTAL_LEN, "<%s>:<%s>:<%s>\n", dt_str, level_str, user_str);
	//should the angle braces be printed literally?  it is not clear to me from the assignment
	
	//write combined string to log
	write (Fd, log_str, strlen(log_str));	
}

int set_logfile ( const char *logfile_name )
{
	debug ("set_logfile(%s)\n",logfile_name);
	
	//  if file already open, close it!
	if (Fd >= 0)
	{
		debug ("closing previous file\n");
		close (Fd);  //alternativly, we could call our close_logfile routine
	}
		
	//ok, now open new file	
	Fd = open (logfile_name, O_CREAT | O_WRONLY | O_APPEND, 0600);
	
	//error check
	if (Fd < 0)
	{
		debug ("error, unable to open file\n");
		return ERROR;
	}
	
	return OK;
}

void close_logfile ( void )
{
	if (Fd >= 0)
	{
		debug ("closing file\n");
		close (Fd);
		Fd = -1;                  //mark descriptor as invalid so we dont try to use it!
	}
	else
	{
		debug ("no file is open, nothing to close\n");
	}
}


void debug ( const char *fmt, ... )
{
	#ifdef DEBUG_ON
	  va_list     ap;
	  va_start(ap, fmt); //init the vararg list
	  vprintf(fmt, ap);
	#endif
}


