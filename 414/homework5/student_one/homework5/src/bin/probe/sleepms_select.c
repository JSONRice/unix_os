
#include <sys/types.h>
#include <sys/time.h>

void
sleepms (int sleep_time)
{
	struct timeval timeout;

	timeout.tv_usec = 1000 * (sleep_time % 1000);
	timeout.tv_sec = sleep_time / 1000;

	if (select (0, 0, 0, 0, &timeout) < 0)
		perror ("select");
	return;
}
