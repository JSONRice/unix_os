/***************************************************************** 
 * Student: Jason Rice
 * 
 * Course: 605.414 System Development in a Unix Environment
 * 
 * File: shared_mem.h
 *
 * Description:
 *    This is the header file that declares the shared memory
 * library functions. The functions in this library are the
 * declarations for advanced IPC functions that create (attach), 
 * detach, and destroy shared volatile memory segments. 
 ****************************************************************/
#ifndef  SHARED_MEM_H
#define	SHARED_MEM_H

#include <vector>
using namespace std;

#define BASE_ADDR 0x000000
#define DATASIZE 20
#define KEY 0x6819

struct data {
	int is_valid;
	float x;
	float y;
};

static struct data *SharedMem;

struct shared_mem_info{
	int shmid;
	void * shmaddr;
	int key;
};



void *connect_shm(int key, int size);
int detach_shm(void *addr);
int destroy_shm(int key);

#endif /* SHARED_MEM_H */
