// 
// homework 4, Unix Systems Programming
// Fall 
//
// file: shared_mem.c
//
// This shared memory library gets built as shmlib.a.  It allows for
// connecting, detaching, and destroying shared memory segments.  for 
// more info, see shmlib.h.  
//
// This library keeps track of all shared memory segments created with
// the connect_shm function.  The segments are stored in an static 
// array, where each element contains a key, an address, and an ID.
// The first call to any function in this library will initialize the
// array.

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include "shmlib.h"
#include "errcodes.h"


#define ANY_SHM_ADDR  0x000000
#define NO_KEY        0

// To keep track of shared memory segments, we need to associate
// a key with an address and a shared memory ID.  The same key
// can have multiple addresses, but we address that with multiple
// entries.  a single entry is as follows:
//
typedef struct _key_entry
{
	key_t  key;
	void*  addr;
	int    shmid;
	
} key_entry_t;

// the following static array keeps all of the key entries:
//
static key_entry_t key_array[MAX_SHM];

void init_data_structures (void);
int  next_avail_key_index (void);
void handle_first_time (void);

void* connect_shm (int key, int size)
{
	int index;
	int shmid;
	void* shm_mem;
	
	handle_first_time ();
	
	index = next_avail_key_index ();
	if(index < 0)
	{
		return NULL;
	}
	
	shmid = shmget (key, size, IPC_CREAT | 0644);
	if(shmid < 0)
	{
		return NULL;
	}
	
	shm_mem = (void *) shmat (shmid, ANY_SHM_ADDR, 0);
	if(shm_mem == NULL)
	{
		return NULL;
	}
	
	// we succeeded. save info so we can detach and or destroy later:
	key_array[index].key  = key;
	key_array[index].addr = shm_mem;
	key_array[index].shmid = shmid;
	
	return shm_mem;
}

void handle_first_time (void)
{
	static char first_time = 1;

	if(first_time)
	{
		init_data_structures ();
		first_time = 0;
	}
}

void init_data_structures (void)
{
	int i;
	for(i=0; i<MAX_SHM; i++)
	{
		key_array[i].key  = NO_KEY;
	}
}

int next_avail_key_index (void)
{
	int i;
	for(i=0; i<MAX_SHM; i++)
	{
		if(key_array[i].key == NO_KEY)
			return i;
	}
	return -1; //no slot available!
}

int detach_shm (void* addr)
{
	int i, err;
	
	err = shmdt (addr);
	if(err == -1)
	{
		return ERROR;
	}
	return OK;
}

int destroy_shm (int key)
{
	int i, shmid, err;
	
	handle_first_time ();
	
	//find shmid, detach all matching addr, and
	//clear all matching key_array entries
	shmid = -1;
	for(i=0; i<MAX_SHM; i++)
	{
		if(key_array[i].key == key)
		{
			shmid = key_array[i].shmid;
			key_array[i].key  = NO_KEY;
			
			shmdt (key_array[i].addr);
			//error check not applicable here
			//addr may already be detached
		}
	}
	
	//if no entries found:
	if(shmid == -1)
	{
		return ERROR;
	}
	
	//now destroy:
	err=shmctl (shmid, IPC_RMID, 0);
	if(err == -1)
	{
		return ERROR;
	}
	
	//success!
	return OK;
}
