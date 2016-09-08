/***************************************************************** 
 * Student: Jason Rice
 * 
 * Course: 605.414 System Development in a Unix Environment
 * 
 * File: liblog_mgr.h
 *
 * Description:
 *    This is the header file that declares the log library 
 * functions. Functions are declared that log events, set the log
 * file, and close the log file. Log levels are included in an
 * enumerator type. Each of the function implementations are
 * atomic meaning that only one log transaction occurs at a given
 * preventing log file corruption.
 ****************************************************************/
#ifndef  LIBLOG_MGR_H
#define	LIBLOG_MGR_H

typedef enum {INFO, WARNING, FATAL} Levels;

#define DEFAULT "logfile"
#define ERROR -1
#define OK 0

int log_event(Levels l, const char *fmt, ...);
int set_logfile(const char *logfile_name);
void close_logfile();

#endif /* LIBLOG_MGR_H */
