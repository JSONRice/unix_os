// 
// homework 3, Unix Systems Programming
// Fall
//
// file: log_mgr.h
// This header is for log_mgr.c which gets built as liblog_mgr.a

#define OK     0
#define ERROR -1
#define DEFAULT_LOGFILE "logfile"

typedef enum {
	INFO, 
	WARNING, 
	FATAL
} Levels;

int log_event ( Levels l, const char *fmt, ... ); 
//
// this function logs a message to a log file.  if a log
// file has not been set, the default is used.  the message
// can be in the same format as the printf family.  the
// message is tagged with the date and a severity level.
// this function uses the system write call which operates
// atomically.  this means that events can be logged by 
// concurrent processes without messages getting lost or
// garbled.
//
// for the record, lowercase "L" is not an ideal variable name.
// it can easily be confused with an uppercase "I" or a number 
// "1".  or perhaps I need to find a better font for my text 
// editor! 

int set_logfile ( const char *logfile_name );
//
// this function will open the given file so that if can be
// used for logged events.  if a log file is already open,
// it will automatically be closed before the new one is opened.

void close_logfile ( void ); 
//
// the function name says it all.  if a logfile is open, then
// it will be closed.  if not, no harm is done.
