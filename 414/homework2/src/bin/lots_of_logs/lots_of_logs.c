#include <unistd.h>
#include <stdlib.h>
#include "log_mgr.h"

int
main()
{
	int i;
	float z = 0.5;

	set_logfile ("BIGlogfile");
	for (i = 0; i < 50; i++)
	{
		log_event (INFO, "[%d]This is test %d for %f", getpid(), i, z);
		log_event (WARNING, "[%d]This is test %d for %f", getpid(), i, z);
		log_event (FATAL, "[%d]This is test %d for %f", getpid(), i, z);
		z = z * 0.98;
	}

	close_logfile();
	exit (0);
}
