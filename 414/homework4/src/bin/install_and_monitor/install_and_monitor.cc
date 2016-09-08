/***************************************************************** 
 * Student: Jason Rice
 * 
 * Course: 605.414 System Development in a Unix Environment
 * 
 * File: install_and_monitor.cc
 *
 * Description:
 *   Combines the logic from install_data and monitor_shm into
 * one multithreaded program. When executed this program spawns
 * an install and monitor thread and saves their state within
 * a threads map (database) for reference. The program ensures
 * the threads cooperate by allowing them to block and complete
 * execution. Each thread is destroyed (deallocated) via a
 * thread destructor that maintains a mutex lock to ensure only
 * one thread gets destroyed at a time.
 ****************************************************************/
#include "log_mgr.h"
#include <map>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>

using namespace std;

/****************************************************************
 ************************** CONSTANTS ***************************
 ***************************************************************/

#define ARG_MAX  3
#define TOK_MAX  4
#define DATASIZE 20
#define LINE_MAX 256

/****************************************************************
 **************************** STRUCTS ***************************
 ***************************************************************/

struct data {
	int is_valid;
	float x;
	float y;
};

// Gets allocated on the heap and added to the 
// Threads database for quick reference:
struct thread_data {
	pthread_t thread;
	// sequential unique id that increments starting at 1
	unsigned int id;
	// 1 for on, else 0 for off. mainly used for testing purposes only.
	int state;
};

struct tuple {
	int index;
	float x;
	float y;
	int inc;
};

/****************************************************************
 **************************** GLOBALS ***************************
 ***************************************************************/
// Monitor seconds:
static int Seconds      = 0;

static struct data GlobalMem[DATASIZE];

static vector<tuple> Tuples;
static map<pthread_t, thread_data> Threads;

static pthread_mutex_t SigLock;
static pthread_mutex_t DestructorLock;
static pthread_t InstallThread;
static pthread_t MonitorThread;
static unsigned int ThreadIncrement = 1;

typedef void *RunFunction(void *);

/****************************************************************
 *********************** HELPER FUNCTIONS ***********************
 ***************************************************************/

// Mainly used for testing (return - 0 failure, 1 success):
int log_threads_map(){
	if (Threads.size() == 0){
		log_event(WARNING, "install_and_monitor:log_threads_map() no threads in map to print");
		return 0;
	}

	map<pthread_t, thread_data>::iterator itr = Threads.begin();
	
	log_event(INFO, 
				 "install_and_monitor:log_threads_map() printing out threads map:");
	for(;itr != Threads.end();itr++){
		log_event(INFO, 
					 "install_and_monitor:log_threads_map() first(key)->pthread_t: %lu", 
					 itr->first);
		log_event(INFO, 
					 "install_and_monitor:log_threads_map() second->thread: %lu", 
					 itr->second.thread);
		log_event(INFO, 
					 "install_and_monitor:log_threads_map() second->id: %d", 
					 itr->second.id);
		log_event(INFO, 
					 "install_and_monitor:log_threads_map() second->state: %d", 
					 itr->second.state);		
	}
	return 1;
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

void usage(const char *str){
	if (strlen(str) > 0){
		printf("%s\n",str);
	}
	printf("\tUsage: install_and_monitor <input data file> <monitor time (seconds)>\n");
}

int isnotempty(const char *str){
	// Is the string empty?
	if (str[0] == '\0'){
		log_event(WARNING, "install_and_monitor:isnotempty() - empty string detected.\n");
		usage("One of the arguments is empty.");
		return 0;
	}
	return 1;
}

int checkargs(int argc, char *argv[]){
	int i = 0;
	size_t strlength = 0;
	if (argc < ARG_MAX){
		log_event(FATAL, "install_and_monitor:checkargs() - Not enough arguments.\n");
		usage("Not enough arguments.");
		exit(1);
	}
	else if (argc > ARG_MAX){
		log_event(FATAL, "install_and_monitor:checkargs() - Too many arguments. %d maximum.\n", ARG_MAX);
		usage("Too many arguments. See logs for more details.");
		exit(1);
	}

	if (isnotempty(argv[1]) && isnotempty(argv[2])){
		Seconds = isvalid(argv[2]);
		if (Seconds < 0){
			log_event(FATAL, "install_and_monitor:checkargs() Monitor time must be non-negative."); 
			usage("Monitor time must be non-negative.");
			exit(1);
		}
	}
	// else
	return 1;
}

// Used mainly for testing purposes:
int print_shm(int t){
	int i = 0;
	log_event(INFO, "install_and_monitor:print_shm() BEGIN time is %d secs", t);
	for(;i < DATASIZE;i++){
		log_event(INFO, "install_and_monitor:print_shm() x:%.2f y:%.2f is_valid:%d", 
					 GlobalMem[i].x, GlobalMem[i].y, GlobalMem[i].is_valid); 
	}
	log_event(INFO, "install_and_monitor:print_shm() END");
}

/****************************************************************
 *********************** MONITOR FUNCTIONS **********************
 ***************************************************************/
// Ping the shared memory block to compute the avg. x, avg. y, and active_count.
void monitor_global_mem(int t){

	// print_shm(t);

	int i = 0;
	// active_count is float to correct integer rounding division:
	float active_count = 0;
	float totalx = 0;
	float totaly = 0;
	float avgx   = 0;
	float avgy   = 0;

	for(;i < DATASIZE;i++){
		if (GlobalMem[i].is_valid){
			totalx += GlobalMem[i].x;
			totaly += GlobalMem[i].y;
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
					 "install_and_monitor:monitor_global_mem() At time %d:%d elements are active:x = %.2f and y = %.2f",
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
		monitor_global_mem(i);
		sleep(second);
	}
}

/****************************************************************
 *********************** INSTALL FUNCTIONS **********************
 ***************************************************************/
void parse(const char * line){
	if (strlen(line) <= 1){
		log_event(WARNING, "install_and_monitor:parse() Skipping empty lines.\n");
		return;
	}

	int i = 0;
	const char * regex = " \t";
	int index   = atoi(strtok((char *) line, regex));
	float x     = atof(strtok(NULL, regex)); 
	float y     = atof(strtok(NULL, regex));
   int inc     = atoi(strtok(NULL, regex));

	log_event(INFO, 
				 "install_and_monitor:parse() Parsed tuple --> index:%d x:%.2f, y:%.2f, inc:%d\n", 
				 index, x, y, inc);

	// Check for invalid:
	if (index < 0 || index > (DATASIZE - 1)){
		log_event(WARNING, 
					 "install_and_monitor:parse() Index of '%d' out of range (0-%d). Not installing.\n", 
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
		log_event(INFO, "install_and_monitor:install() tuple_inc -> %d", tuple_inc);
		// If increment time is non-negative sleep for increment 
		// time then set is_valid to 'true' (1):
		if (tuple_inc > 0 || tuple_inc == 0){
			log_event(INFO, 
						 "install_and_monitor:install() Increment time is non-negative. Installing in %d secs\n", 
						 tuple_inc);

			sleep(tuple_inc);
			// Install to memory since is_valid is now 'true' (1):
			GlobalMem[Tuples.at(i).index].x        = Tuples.at(i).x;
			GlobalMem[Tuples.at(i).index].y        = Tuples.at(i).y;
			GlobalMem[Tuples.at(i).index].is_valid = 1;
			log_event(INFO, 
						 "install_and_monitor:install() Installed block --> [%d](x, y, is_valid): %.2f, %.2f, %d\n",
						 Tuples.at(i).index, Tuples.at(i).x, Tuples.at(i).y, 1);			
		}
		// Else increment time is negative, sleep for 
		// absolute increment time and set is_valid to 'false' (0):
		else {
			abs_secs = abs(tuple_inc);
			log_event(INFO, 
						 "install_and_monitor:install() Increment time is negative (invalid). Sleeping for %d secs\n", 
						 abs_secs);

			sleep(abs_secs);
			GlobalMem[Tuples.at(i).index].is_valid = 0;
			log_event(INFO, 
						 "install_and_monitor:install() Did not install block (invalid). Set is_valid to 0 at [%d]\n", 
						 Tuples.at(i).index);
		}
	}
}

// Parse through the input file and store data in Tuples.cache.
// This must be called prior to the install thread.
void ingest(const char *fname){
	if (!isnotempty(fname)){
		log_event(WARNING, "install_and_monitor:ingest() The input file is empty.\n");
		return;
	}

	FILE *fp = fopen(fname,"r");

	if (!fp){
		perror ("fopen");
		log_event(FATAL, 
					 "install_and_monitor:ingest() '%s' failed to read. Check permissions.\n", 
					 fname);
		exit(1);
	}


	log_event(INFO, 
				 "install_and_monitor:ingest() Clearing current bytes (setting to zeros).\n");
	int i = 0;
	for (;i < DATASIZE;i++){
		GlobalMem[i].is_valid = 0;
		GlobalMem[i].x        = 0;
		GlobalMem[i].y        = 0;
	}

	char line[LINE_MAX];

	log_event(INFO, 
				 "install_and_monitor:ingest() Parsing '%s' into vector<tuple> Tuples.\n", fname);

	// Parse lines and store each parsed line data into the tuple vector:
	while(fgets(line,LINE_MAX,fp) != NULL){
		if (strlen(line) <= 0){
			continue;
	   }
		parse(line);
	}

	if(fclose(fp)){
		log_event(WARNING, "install_and_monitor:ingest() fclose error\n");
	}
}

/****************************************************************
 *********************** THREAD FUNCTIONS ***********************
 ***************************************************************/

// This function checks if the Install and Monitor threads are running:
void ping_threads(unsigned int checkinstall, 
						unsigned int checkmonitor){
	if (checkinstall == 0 && checkmonitor == 0){
		log_event(WARNING,
					 "install_and_monitor:ping_threads() no check flags were set. Review ping_threads function.");
		// preempt
		return;
	}

	if (checkinstall > 0){
		// Did the install thread spawn?
		if (InstallThread == pthread_t(-1)){
			printf("install_and_monitor:ping_threads() Fatal error. Install thread failed to generate.\n");
			log_event(FATAL, 
						 "install_and_monitor:ping_threads() Fatal error. Install thread failed to generate.");
			exit(1);
		}
		else {
			log_event(INFO,
						 "install_and_monitor:ping_threads() Install thread: '%lu' spawned!", 
						 InstallThread);
		}
	}

	if(checkmonitor > 0){
		// Did the monitor thread spawn?
		if (MonitorThread == pthread_t(-1)){
			printf("install_and_monitor:ping_threads() Fatal error. Monitor thread failed to generate.\n");
			log_event(FATAL, 
						 "install_and_monitor:ping_threads() Fatal error. Monitor thread failed to generate.");
			exit(1);
		}
		else {
			log_event(INFO,
						 "install_and_monitor:ping_threads() Monitor thread: '%lu' spawned!", MonitorThread);
		}
	}
}

// Apply mutex locks to clean up thread. The reason why mutex locks are applied
// is to ensure only one thread is destructed at a time (overcome race conditions):
int destructor(){
	pthread_mutex_lock(&DestructorLock);

	// acquire the calling thread:
	pthread_t thread = pthread_self();

	map<pthread_t, thread_data>::iterator itr = Threads.find(thread);
	if (itr != Threads.end()){
		itr->second.state = 0;
		log_event(INFO, 
					 "install_and_monitor:destructor() thread with id '%d' state is now set to '%d' for being destructed (inactive).", 
					 itr->second.id, itr->second.state);
	}
	
	// done with the lock. release it:
	pthread_mutex_unlock(&DestructorLock);

	log_event(INFO, "install_and_monitor:destructor() terminating the thread with pthread_exit()");
	pthread_exit((void *) thread);

	// thread should not get here. if it does report an error:
	log_event(FATAL, "install_and_monitor:destructor() thread '%lu' did not destruct.", thread);
	return 0;
}

void * installing(void * arg){
	install();
	destructor();
	return NULL;
}

void * monitoring(void * arg){
	monitor(Seconds);
	destructor();
	return NULL;
}

// Waits the current active thread until this thread (argument) 
// terminates (finishes). Return 0 for error, else return 1.
int wait(pthread_t thread){

	log_event(INFO, "install_and_monitor:wait() BEGIN");

	// lookup thread in our thread map. should be there:
	map<pthread_t, thread_data>::iterator itr = Threads.find(thread);

	if (itr == Threads.end()){
		log_event(INFO,
					 "install_and_monitor:wait() pthread_t '%lu' not found in map.", thread);
		perror("return 0");
		return 0;
	}

	// If successful, the pthread_join() function shall return zero; 
	// otherwise, an error number shall be returned to indicate the error. see pthread.h
	
	if (pthread_join(thread, NULL) != 0){
		perror("pthread_join");
		log_event(WARNING, 
					 "install_and_monitor:wait() pthread_join failed for thread with id '%d'", 
					 thread, itr->second.id);
		return 0;
	}
	

	// Make sure to update the map (threads database) so we don't attempt to reblock/rewait:
	itr = Threads.find(thread);
	if (itr != Threads.end()){
		Threads.erase(itr);
	}
	else {
		log_event(WARNING,
					 "install_and_monitor:wait() failed to update threads db. Review wait()");
	}	
	return 1;
}

// Spawn and run thread and set it's state in the threads map. 
// Return the thread up to calling function (main).
// A -1 is returned if the thread failed to spawn.
pthread_t run(RunFunction start_routine){

	thread_data current;

	// Generate thread and save off to map:
	if (pthread_create(&current.thread, NULL, 
							 start_routine, (void*) (&current))){
		perror("pthread_create");
		current.state = 0;
		log_event(FATAL, "install_and_monitor:run() pthread_create for thread with id of '%d' failed", 
					 current.id);
		return -1;
	}
	// else: spawned active - set state to 1
	current.id = ThreadIncrement++;
	current.state = 1;
	log_event(INFO, 
				 "install_and_monitor:run() thread: %lu <-> id: '%d' <-> *thread:%lu* spawned", 
				 current, current.id, current.thread);

	// store in database
	Threads[current.thread] = current;
	return pthread_t(current.thread);
}

// block until each thread has completed processing (return 1 for success else 0):
int block_and_wait(){
	log_event(INFO, "install_and_monitor:block_and_wait() blocking threads until they are all done...");

	if (Threads.size() == 0){
		log_event(WARNING, "install_and_monitor:block_and_wait() no more threads were found to block. All done!");
		return 0;
	}
	while (Threads.begin() != Threads.end()){
		map<pthread_t, thread_data>::iterator itr = Threads.begin(); 
		for(;itr != Threads.end(); ++itr){
			if (wait(itr->first)){
				log_event(INFO, 
							 "install_and_monitor:block_and_wait() the thread with id '%d' finished blocking", 
							 itr->second.id);
			}
			else {
				log_event(WARNING, 
							 "install_and_monitor:block_and_wait() the following thread failed to block %lu", 
							 itr->first);
			}
		}
	}
	// Every thread blocked (waited) properly through pthread_join():
	log_event(INFO, "install_and_monitor:block_and_wait() threads have all blocked");
	return 1;
}

// Set locks to default settings (see pthread.h):
void init_locks(){
	if (pthread_mutex_init(&DestructorLock, NULL) != 0){
		log_event(WARNING, "install_and_monitor:init_locks() Failed to set destructor lock.");
	}
	else {
		log_event(INFO, "install_and_monitor:init_locks() Initialized destructor lock.");
	}

	if (pthread_mutex_init(&SigLock, NULL) != 0){
		log_event(WARNING, "install_and_monitor:init_locks() Failed to set signal lock.");
	}
	else {
		log_event(INFO, "install_and_monitor:init_locks() Initialized signal lock.");
	}
}


void destroy_locks(){
	if (pthread_mutex_destroy(&DestructorLock) != 0){
		log_event(WARNING, "install_and_monitor:destroy_locks() Failed to remove destructor lock.");
	}
	else {
		log_event(INFO, "install_and_monitor:destroy_locks() Removed destructor lock.");
	}

	if (pthread_mutex_destroy(&SigLock) != 0){
		log_event(WARNING, "install_and_monitor:destroy_locks() Failed to remove signal lock.");
	}
	else {
		log_event(INFO, "install_and_monitor:destroy_locks() Removed signal lock.");
	}
}

/****************************************************************
 *********************** SIGNAL FUNCTIONS ***********************
 ***************************************************************/
void sigtermhandle(int signo){
	pthread_mutex_lock(&SigLock);
	if (signo == SIGTERM){
		log_event(INFO, 
					 "install_and_monitor:sigtermhandle() Caught SIGTERM - Stop install thread, clear memory, and exit:");

		// uncomment for additional testing:
		// log_threads_map();

		// we should have this thread in our database:
		map<pthread_t, thread_data>::iterator itr;
		itr = Threads.find(InstallThread);
		if (itr == Threads.end()){
			log_event(WARNING, "install_and_monitor:sigtermhandle() Did not find install thread '%lu' in database. Hasn't spawned yet.", 
						 InstallThread);
		}

		if (pthread_cancel(InstallThread) != 0){
			log_event(WARNING, "install_and_monitor:sigtermhandler() Install thread failed to stop.");
		}
		else {
			log_event(INFO, "install_and_monitor:sigtermhandler() Install thread stopped.");
		}

		int i = 0;
		for (;i < DATASIZE;i++){
			GlobalMem[i].is_valid = 0;
			GlobalMem[i].x        = 0;
			GlobalMem[i].y        = 0;
		}

		log_event(INFO, "install_and_monitor:sigtermhandler() Exit program.");
		exit(0);
	}
	else {
		log_event(WARNING, 
					 "install_and_monitor:sigtermhandle() Only registered SIGHUP signal. Instead found sig: %d", signo);
	}
	pthread_mutex_unlock(&SigLock);
}



void sighuphandle(int signo){
	pthread_mutex_lock(&SigLock);
	if (signo == SIGHUP){
		log_event(INFO, 
					 "install_and_monitor:sighuphandle() Caught SIGHUP. Stop install thread, clear mem, and reinstall:");

		// uncomment for additional testing:
		// log_threads_map();

		// we should have this thread in our database:
		map<pthread_t, thread_data>::iterator itr;
		itr = Threads.find(InstallThread);
		if (itr == Threads.end()){
			log_event(WARNING, "install_and_monitor:sighuphandle() Did not find install thread in database. Hasn't spawned yet.");
		}
		else if (pthread_cancel(InstallThread) != 0){
			log_event(WARNING, "install_and_monitor:sighuphandler() Install thread failed to stop.");
		}
		else {
			itr->second.state = 0;
			log_event(INFO, "install_and_monitor:sighuphandler() Install thread stopped.");
		}

		int i = 0;
		for(;i < DATASIZE;i++){
			GlobalMem[i].is_valid = 0;
			GlobalMem[i].x        = 0;
			GlobalMem[i].y        = 0;
		}

		// Restart:
		log_event(INFO, "install_and_monitor:sighuphandler() Restarting install thread.");
		InstallThread = run(installing);
		ping_threads(1,0);
	}
	else {
		log_event(WARNING, 
					 "install_and_monitor:sighuphandle() Expected a SIGHUP signal. Instead found signo: %d", 
					 signo);
	}
	pthread_mutex_unlock(&SigLock);
}

void registersighandle(void (*handle)(int), const unsigned int code){
	if (signal(code, handle) == SIG_ERR){
		log_event(WARNING, 
					 "install_and_monitor:registersighandle() Unable to register sig code '%u' Make sure it's valid.", 
					 code);
	}
	else{
		log_event(INFO, "install_and_monitor:registersighandle() Signal with code '%u' is registered.", code);
	}
}

/****************************************************************
 ************************* MAIN FUNCTION ************************
 ***************************************************************/
int main(int argc, char* argv[]){
	set_logfile("install_and_monitor.log");
	log_event(INFO, "install_and_monitor:main() BEGIN");

	init_locks();

	// register signal handling:
	registersighandle(&sigtermhandle, SIGTERM);
	registersighandle(&sighuphandle, SIGHUP);

	// parse, ingest, and set memory:
	if (checkargs(argc, argv)) {
		ingest(argv[1]);
	}

	// Spawn install and monitor threads:

	log_event(INFO, "install_and_monitor:main() Spawning install thread.");
   InstallThread = run(installing);
   log_event(INFO, "install_and_monitor:main() Spawning monitor thread.");
   MonitorThread = run(monitoring);
	ping_threads(1,1);

	// Wait for all threads to finish running
	// keep blocking and waiting until they are all done:
	while(block_and_wait());

	destroy_locks();
	
	log_event(INFO, "install_and_monitor:main() END");
	close_logfile();
	exit(0);
}
