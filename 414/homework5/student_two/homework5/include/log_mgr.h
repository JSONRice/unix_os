#ifndef _LOG_MGR_H
#define _LOG_MGR_H

#define OK         0
#define ERROR      -1
#define MAXLENGTH  256
#define MINLENGTH  25
#define LOGFILE    "logfile"

typedef enum {INFO, WARNING, FATAL} Levels;

int log_event (Levels l, const char *fmt, ...);
int set_logfile (const char *logfile_name);
void close_logfile(void);
int convert_level_to_text(char *textValue, Levels l);

#endif
