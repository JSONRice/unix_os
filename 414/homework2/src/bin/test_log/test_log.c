#include <unistd.h>
#include <stdlib.h>
#include "log_mgr.h"

int
main()
{
	int i;
	float z = 0.5;

	for (i = 0; i < 5; i++)
	{
		log_event (INFO, "[%d]This is test %d for %f", getpid(), i, z);
		z = z * 0.98;
	}
	set_logfile ("LOGnewlog");

	for (i = 0; i < 5; i++)
	{
		log_event (FATAL, "[%d]%s %d for %f",
					getpid(), "THIS IS TEST", i, z);
		z = z * 0.98;
	}

	set_logfile ("LOGwarnlog");
	for (i = 0; i < 5; i++)
		log_event (WARNING, "[%d]%c%c%c%c", getpid(), 't', 'e', 's', 't');
	close_logfile();
	exit (0);
}
