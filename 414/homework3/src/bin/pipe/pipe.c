/***************************************************************** 
 * Student: Jason Rice
 * 
 * Course: 605.414 System Development in a Unix Environment
 * 
 * File: pipe.c
 *
 * Description:
 *    This program applies Unix pipes (IPC) to combine the output
 * of a given program (first argument) as the input to the second
 * program (second argument). This is analogous to the Unix/Linux
 * '|' operator. In other words the functionality is the same.
 *
 * For example: %pipe "cat /etc/passwd" "wc -l"
 * Is equivalent to: cat /etc/passwd | wc -l
 *
 * The program also handles child processes with flags as noted 
 * in the above example. To accomplish string tokenization is 
 * applied.
 ****************************************************************/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "log_mgr.h"

const int FDS_MAX  = 2;
const int ARG_MAX  = 3;
const int TOK_MAX = 20;

void usage(const char *str){
	if (strlen(str) > 0){
		printf("%s\n",str);
	}
	printf("\tUsage: pipe <program string> <program string>\n");
}

int valid(const char *str){
	// Is the string empty?
	if (str[0] == '\0'){
		log_event(WARNING, "pipe:valid() - empty string detected.");
		usage("One of the arguments is empty.");
		return 0;
	}
	return 1;
}

int checkargs(int argc, char *argv[]){
	int i = 0;
	size_t strlength = 0;
	if (argc < ARG_MAX){
		log_event(FATAL, "pipe:checkargs() - Not enough arguments.");
		usage("Not enough arguments.");
		return 0;
	}
	else if (argc > ARG_MAX){
		log_event(FATAL, "pipe:checkargs() - Too many arguments. Two maximum.");
		usage("Too many arguments. Two maximum.");
		return 0;
	}
	return (valid(argv[1]) & valid(argv[2]));
}

void fatal(const char* msg, const int code){
	perror(msg);
	log_event(FATAL,msg);
	exit(code);
}

void pipeForkRun(const char *program, int std, const int *fds){
	char * tokens[TOK_MAX];

	switch(fork()){
		case 0:
			log_event(INFO, "pipe:pipeForkRun() - Executing the following process as child: %s",program);

			// Keep parsing tokens until none are left:
			int i = 0;
			tokens[i] = strtok((char *) program, " \t");
			for (i = 1;i < TOK_MAX;i++){
				tokens[i] = strtok(NULL, " \t");
				if(tokens[i] == NULL){
					break;
				}
			}

			close(std);
			dup(fds[std]);
			close(fds[0]);
			close(fds[1]);
			execvp(tokens[0], tokens);
			// If the code below executes then we have an error.
			perror("pipe:pipeForkRun() - Error executing child process.");
			// Force exit to kernel.
			_exit(1);
			break;
		case -1:
			fatal("pipe:pipeForkRun() - Error forking a process for the pipe.", 1);
			break;
	}
	// If fork returns a positive number this will be the pid from the parent process.
	// In other words that's the process belonging to this pipe program with a main.
}

void pipeClose(const int *pfds){
	// Close file descriptors and wait for each process to finish:
	log_event(INFO, "pipe:pipeClose() - Closing the pipe file descriptors.");
	close(pfds[0]);
	close(pfds[1]);
	log_event(INFO, "pipe:pipeClose() - Pipe file descriptors close.");
	log_event(INFO, "pipe:pipeClose() - Waiting for processes to complete.");
	while (wait(NULL) != -1);
}

void pipeRun(const char *pOne, const char *pTwo){
	// Set up pipe file descriptors. 
	// pfds[0] is set for reading. 
	// pfds[1] is set for writing.
	int pfds[FDS_MAX];

	if (pipe(pfds) == -1){
		fatal("pipe:pipeRun() - Error creating pipe.", 1);
	}
	log_event(INFO, "pipe:pipeRun() - Successfuly opened pipe. Preparing to fork process.");

	// Fork two child processes and have them write to and read from the pipe.
	pipeForkRun(pOne, 1, pfds);
	pipeForkRun(pTwo, 0, pfds);

	// Complete and close the pipe I/O transaction
	pipeClose(pfds);

	log_event(INFO, "pipe:pipeRun() - Pipe transaction is complete.");
}

int
main(int argc, char *argv[]){
	set_logfile("pipe.log");
	log_event(INFO, "pipe:main() BEGIN");
	if (checkargs(argc, argv)) {
		pipeRun(argv[1], argv[2]);
	}
	log_event(INFO, "pipe:main() END");
	close_logfile();
	exit(0);
}
