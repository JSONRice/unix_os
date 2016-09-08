#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "log_mgr.h"
#define		NUM_MSGS		100
#define		LOGFILE_NAME_LEN	80

int
main()
{
	int i;
	float z = 0.5;
	char logfilename[LOGFILE_NAME_LEN];

	for (i = 0; i < NUM_MSGS; i++)
	{
		set_logfile ("LOGnewlog");
		log_event (INFO, "[%d]This is test %d for %f", getpid(), i, z);
		z = z * 0.98;
	}

	z = 0.6;
	for (i = 0; i < NUM_MSGS; i++)
	{
		sprintf (logfilename, "LOGFILE#%d", i);
		set_logfile (logfilename);
		if (i < NUM_MSGS / 2)
			log_event (WARNING,
				"[%d]This is test %d for %f",
				getpid(), i, z);
		else
			log_event (FATAL,
				"[%d]This is test %d for %f",
				getpid(), i, z);
		z = z * 0.98;
	}
	exit (0);
}
