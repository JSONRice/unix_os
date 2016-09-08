/******************************************************************************* 
 * Student: Jason Rice
 * 
 * Course: 605.414 System Development in a Unix Environment
 * 
 * File: monitor_shm.cc
 *
 * Description:
 *   This program connects to the shared memory block generated
 * by install_data and monitors the tuples recorded to the memory
 * block. Monitor pings are conducted every second during the
 * specified time interval. If there is one or more active (valid)
 * entry the ping reveals the following:
 * 
 * At time <second>:<active_count> elements are active:x = <xavg> and y = <yavg> 
 *
 * Else if all entries are inactive (invalid):
 *
 * At time <second>:no elements are active
 *******************************************************************************/
#include "log_mgr.h"
#include "shared_mem.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>

#define MON_DEFAULT 30

void usage(const char *str){
	if (strlen(str) > 0){
		printf("%s\n",str);
	}
	printf("\tUsage: monitor_shm <monitor time (seconds)>\n");
}

// Convert string to long then cast up to an int.
int isvalid(char *arg){
	long val = 0;
	char *next;
	// Get value with failure detection.
	val = strtol (arg, &next, 10);
	// Check for empty string and characters left after conversion:
	if ((next == arg) || (*next != '\0')) {
		return 0;
	} 
	else {
		return val;
	}
}

// Used mainly for testing purposes:
int print_shm(int t){
	int i = 0;
	log_event(INFO, "monitor_shm:print_shm() BEGIN time is %d secs", t);
	for(;i < DATASIZE;i++){
		log_event(INFO, "monitor_shm:print_shm() x:%.2f y:%.2f is_valid:%d", 
					 SharedMem[i].x, SharedMem[i].y, SharedMem[i].is_valid); 
	}
	log_event(INFO, "monitor_shm:print_shm() END");
}

// Ping the shared memory block to compute the avg. x, avg. y, and active_count.
void ping_shm(int t){

	// print_shm(t);

	int i = 0;
	// active_count is float to correct integer rounding division:
	float active_count = 0;
	float totalx = 0;
	float totaly = 0;
	float avgx   = 0;
	float avgy   = 0;

	for(;i < DATASIZE;i++){
		if (SharedMem[i].is_valid){
			totalx += SharedMem[i].x;
			totaly += SharedMem[i].y;
			active_count += 1.0;
		}
	}
	// Compute averages and print to screen:
	if (active_count){
		avgx = totalx / active_count;
		avgy = totaly / active_count;
		printf("At time %d:%d elements are active:x = %.2f and y = %.2f\n", 
				 t, (int) active_count, avgx, avgy);
		log_event(INFO, 
					 "monitor_shm:ping_shm() At time %d:%d elements are active:x = %.2f and y = %.2f",
					 t, (int) active_count, avgx, avgy);
	}
	else {
		printf("At time %d:no elements are active\n", t);
		log_event(INFO, "At time %d:no elements are active", t);
	}
}

void monitor(const unsigned int secs){
	int i = 0;
	int second = 1;
	for (;i < (secs + 1);i++){
		ping_shm(i);
		sleep(second);
		// while((second = sleep(second)));
	}
}

int main(int argc, char * argv[]){
	set_logfile("monitor_shm.log");
	log_event(INFO, "monitor_shm:main() BEGIN\n");

   int secs = 0;

	// Check for optional arg (seconds to monitor):
	if (argc > 1){
		secs = isvalid(argv[1]);
		if (secs >= 0){
			log_event(INFO, 
						 "monitor_shm:main() Detected optional arg. Overriding default monitor time from %d to %d secs.\n", 
						 MON_DEFAULT, secs);
		}
		else {
			usage("Seconds was found to be negative.");
		}
	}
	else {
		secs = MON_DEFAULT;
		log_event(INFO, "monitor_shm:main() Default monitor time set to %d secs.\n", secs);
	}

	size_t size = sizeof(struct data) * DATASIZE;
   log_event(INFO, "monitor_shm:main() Reading from %d bytes of shared memory.\n", size);
	log_event(INFO, "monitor_shm:main() Connecting to shared memory with key of %d", KEY);
	SharedMem = (struct data *) connect_shm(KEY, size);
	if (SharedMem == NULL){
      perror ("shmat");
      log_event(FATAL, "monitor_shm:main() Shared memory failed to allocate.\n");
      exit(1);
   }
	
	monitor(secs);
	
	log_event(INFO, "monitor_shm:main() END\n");
	close_logfile();
	exit(0);
}
