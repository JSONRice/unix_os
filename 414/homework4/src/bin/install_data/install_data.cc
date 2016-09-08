/*********************************************************************************
 * Student: Jason Rice
 * 
 * Course: 605.414 System Development in a Unix Environment
 * 
 * File: install_data.cc
 *
 * Description:
 *   Parse out the lines and install (write) to the shared memory.
 * Each line of the file will follow the format:
 * <index> <x_value> <y_value> <time increment>
 * 
 * <index> : ranges 0-19 indicates the array element (index) of shared memory.
 * <x_value> : floating point number
 * <y_value> : floating point number
 * <time interval> : If non-negative represents number of seconds of delay (sleep)
 *                   before data is written to memory. If negative the absolute
 *                   value represents number of seconds of delay (sleep) before
 *                   setting the corresponding index as invalid. 
 ********************************************************************************/
#include <fcntl.h>
#include "log_mgr.h"
#include "shared_mem.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>
#include <vector>
using std::vector;

#define ARG_MAX  2
#define TOK_MAX  4
#define LINE_MAX_CHARS 256

int MemAllocated = 0;

// Represents a row within the input file to parse:
struct tuple{
	int index;
	float x;
	float y;
	int inc;
};

// Global vector is an easy approach to parsing the input file:
vector<tuple> Tuples;

void sigtermhandle(int signo){
	if (signo == SIGTERM){
		log_event(INFO, 
					 "install_data:sigtermhandle() Caught SIGTERM - Deleting all shared memory and exiting.");
		
		if (!destroy_shm(KEY)){
			log_event(FATAL, "install_data:sigtermhandle() Failed to remove shared memory.");
			exit(1);
		}
		
		exit(0);
	}
	else {
		log_event(WARNING, 
					 "install_data:sigtermhandle() Expected a SIGTERM signal. Instead found signo: %d", 
					 signo);
	}
}

void sighuphandle(int signo){
	if (signo == SIGHUP){
		log_event(INFO, 
					 "install_data:sighuphandle() Caught SIGHUP - Re-establishing shared memory");
		size_t size = sizeof(struct data) * DATASIZE;
		log_event(INFO,
					 "install_data:sighuphandle() Allocating %d bytes of shared memory.\n", 
					 size);

		// Assign the block of memory to the struct pointer. This is an array of 20 data structs.                                               
		log_event(INFO, "install_data:sighuphandle() Connecting to shared memory using key %d", KEY);
		SharedMem = (struct data *) connect_shm(KEY,size); 

		// Make sure to reset memory or all our efforts are in vain:

		if (SharedMem == NULL){
			perror ("shmat");
			log_event(FATAL, "install_data:sighuphandle() Shared memory failed to allocate.\n");
			exit(1);
		}
		// else (memory allocated flag used by helper functions)
		MemAllocated = 1;

		log_event(INFO, "install_data:sighuphandle() Clearing current bytes (setting to zeros).\n");
		int i = 0;
		for (;i < DATASIZE;i++){
			SharedMem[i].is_valid = 0;
			SharedMem[i].x        = 0;
			SharedMem[i].y        = 0;
		}
	}
	else {
		log_event(WARNING, 
					 "install_data:sighuphandle() Only registered SIGHUP signal. Instead found sig: %d", signo);
	}
}

void registersighandle(void (*handle)(int), const unsigned int code){
	if (signal(code, handle) == SIG_ERR){
		log_event(WARNING, 
					 "install_data:registersighandle() Unable to register sig code '%u' Make sure it's valid.", 
					 code);
	}
	else{
		log_event(INFO, "install_data:registersighandle() Signal with code '%u' is registered.", code);
	}
}

// If there's an error let the calling function print out any error message.
// This function is ad-hoc and just destructs. Return 1 for success and 0 for failure.
int destructor(){
	if (!destroy_shm(KEY)){
		return 0;
	}
	return 1;
}

void usage(const char *str){
	if (strlen(str) > 0){
		printf("%s\n",str);
	}
	printf("\tUsage: install_data <input data file>\n");
}

int isnotempty(const char *str){
	// Is the string empty?
	if (str[0] == '\0'){
		log_event(WARNING, "install_data:isnotempty() - empty string detected.\n");
		usage("One of the arguments is empty.");
		return 0;
	}
	return 1;
}

void parse(const char * line){
	// If no memory is allocated preempt:
	if (!MemAllocated){
		log_event(WARNING, "install_data:install() Memory allocation failed. Check program.\n");
		return;
	}
	else if (strlen(line) <= 1){
		log_event(WARNING, "install_data:install() Skipping empty lines.\n");
		return;
	}

	const char * regex = " \t";
	int index   = atoi(strtok((char *) line, regex));
	float x     = atof(strtok(NULL, regex)); 
	float y     = atof(strtok(NULL, regex));
   int inc     = atoi(strtok(NULL, regex));

	log_event(INFO, "install_data:parse() Parsed tuple --> index:%d x:%.2f, y:%.2f, inc:%d\n", 
				 index, x, y, inc);

	// Check for invalid:
	if (index < 0 || index > (DATASIZE - 1)){
		log_event(WARNING, "install_data:parse() Index of '%d' out of range (0-%d). Not installing.\n", 
					 index, (DATASIZE - 1));
		return;
	}

	// Push the parsed line (row) onto tuples vector:
	struct tuple row;
	row.index = index;
	row.x = x;
	row.y = y;
	row.inc = inc;
	Tuples.push_back(row);
}

void install(){
	int abs_secs = 0;
	int i = 0;
	int tuples_count = Tuples.size();
	int tuple_inc = 0;
	for(; i < tuples_count; i++){
		tuple_inc = Tuples.at(i).inc;
		log_event(INFO, "install_data:install() tuple_inc -> %d", tuple_inc);
		// If increment time is non-negative sleep for increment 
		// time then set is_valid to 'true' (1):
		if (tuple_inc > 0 || tuple_inc == 0){
			log_event(INFO, 
						 "install_data:install() Increment time is non-negative. Installing in %d secs\n", 
						 tuple_inc);

			sleep(tuple_inc);
			// Install to memory since is_valid is now 'true' (1):
			SharedMem[Tuples.at(i).index].x        = Tuples.at(i).x;
			SharedMem[Tuples.at(i).index].y        = Tuples.at(i).y;
			SharedMem[Tuples.at(i).index].is_valid = 1;
			log_event(INFO, 
						 "install_data:install() Installed block --> [%d](x, y, is_valid): %.2f, %.2f, %d\n",
						 Tuples.at(i).index, Tuples.at(i).x, Tuples.at(i).y, 1);			
		}
		// Else increment time is negative, sleep for 
		// absolute increment time and set is_valid to 'false' (0):
		else {
			abs_secs = abs(tuple_inc);
			log_event(INFO, 
						 "install_data:install() Increment time is negative (invalid). Sleeping for %d secs\n", 
						 abs_secs);

			sleep(abs_secs);
			SharedMem[Tuples.at(i).index].is_valid = 0;
			log_event(INFO, 
						 "install_data:install() Did not install block (invalid). Set is_valid to 0 at [%d]\n", 
						 Tuples.at(i).index);
		}
	}
}

/* Ingest the input data and place (install) it into a shared memory location. */
void ingest(const char *fname){
	if (!isnotempty(fname)){
		log_event(WARNING, "install_data:ingest() The input file is empty.\n");
		return;
	}

	FILE *fp = fopen(fname,"r");

	if (!fp){
		perror ("fopen");
		log_event(FATAL, "install_data:ingest() '%s' failed to read. Check permissions.\n", fname);
		exit(1);
	}

	size_t size = sizeof(struct data) * DATASIZE;
	log_event(INFO, "install_data:ingest() Allocating %d bytes of shared memory.\n", size);

	// Assign the block of memory to the struct pointer. This is an array of 20 data structs.
	log_event(INFO, "install_data:ingest() Connecting to shared memory using key %d", KEY);
	SharedMem = (struct data *) connect_shm(KEY,size);

	if (SharedMem == NULL){
		perror ("shmat");
		log_event(FATAL, "install_data:ingest() Shared memory failed to allocate.\n");
		exit(1);
	}
	// else (memory allocated flag used by helper functions)
	MemAllocated = 1;

	log_event(INFO, "install_data:ingest() Clearing current bytes (setting to zeros).\n");
	int i = 0;
	for (;i < DATASIZE;i++){
		SharedMem[i].is_valid = 0;
		SharedMem[i].x        = 0;
		SharedMem[i].y        = 0;
	}

	char line[LINE_MAX_CHARS];

	log_event(INFO, "install_data:ingest() Ingesting '%s' into memory block.\n", fname);

	// Parse lines and write to memory:
	while(fgets(line,LINE_MAX_CHARS,fp) != NULL){
		if (strlen(line) <= 0){
			continue;
	   }
		parse(line);
	}

	if(fclose(fp)){
		log_event(WARNING, "install_data:ingest() fclose error\n");
	}
}

int checkargs(int argc, char *argv[]){
	if (argc < ARG_MAX){
		log_event(FATAL, "install_data:checkargs() - Not enough arguments.\n");
		usage("Not enough arguments.");
		exit(1);
	}
	else if (argc > ARG_MAX){
		log_event(FATAL, "install_data:checkargs() - Too many arguments. %d maximum.\n", ARG_MAX);
		usage("Too many arguments. See logs.");
		exit(1);
	}
	return isnotempty(argv[1]);
}

int main(int argc, char * argv[]){
	set_logfile("install_data.log");
	log_event(INFO, "install_data:main() BEGIN\n");

	// register signal handling:
   registersighandle(&sigtermhandle, SIGTERM);
   registersighandle(&sighuphandle, SIGHUP);

	// parse, ingest, and set memory:
	if (checkargs(argc, argv)) {
		ingest(argv[1]);
	}

	// This is where all the magic happens:
	install();

	// all done. destruct:
	if (!destructor()){
		log_event(WARNING, "install_data:main() destructor reports memory failed to dealloc.");
	}
	else {
		log_event(WARNING, "install_data:main() destructor reports memory freed. All done!");
	}

	log_event(INFO, "install_data:main() END\n");
	close_logfile();
	exit(0);
}
