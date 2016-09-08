/***************************************************************** 
 * Student: Jason Rice
 * 
 * Course: 605.414 System Development in a Unix Environment
 * 
 * File: shared_mem.cc
 *
 * Description:
 *    Implementation file for the shared memory include library.
 * This file implements (builds) the code base for the advanced
 * IPC functions that include creating (attaching), detaching,
 * and dellocating the shared volatile memory used for IPC.
 ****************************************************************/
#include "shared_mem.h"
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>

vector<shared_mem_info> SharedMemInfo;

void *connect_shm(int key, int size){
	int shmid;
	int i;

	// Allocate a block of shared memory for IPC. 
	// Create the segment if it doesn't already exist in the kernel. 
	if ((shmid = shmget (key, size, IPC_CREAT | 0666)) < 0){
		perror("shared_mem:connect_shm() shmget");
		return NULL;
	}

	// Return a pointer (address) to the shared memory block. 
	// Let the system assign the address.
	void * addr = shmat(shmid, BASE_ADDR, 0);

	int * val = (int *) addr;
	if (*val < 0){
		perror("shared_mem:connect_shm() shmat");
		return NULL;
	}

	// Store on a vector for later retrieval (detach/destroy):
	shared_mem_info info;
	info.key = key;
	info.shmid = shmid;
	info.shmaddr = addr;
	SharedMemInfo.push_back(info);

	return addr;
}

// Just detach but don't delete shared memory segment. 
// Return 1 for success and 0 for failure.
int detach_shm(void *addr){
	// Nothing to detach, hence the failure:
	if (SharedMemInfo.size() <= 0){
		return 0;
	}

	vector<shared_mem_info>::iterator itr = SharedMemInfo.begin();
	for(;itr != SharedMemInfo.end(); itr++){

		// if the address is in the vector detach shared memory segment located at addr:
		if (itr->shmaddr == addr){
			int ret_val = shmdt(addr);

			if (ret_val != 0){

				perror("shared_mem:detach_mem() shmdt - memory unable to detach");
				return 0;
			}

			SharedMemInfo.erase(itr);
			break;
		}
	}

	return 1;
}

// Detach and delete shared memory segment.
// Return 1 for success and 0 for failure.
int destroy_shm(int key){
	// empty vector (should never happen):
	if (SharedMemInfo.size() <= 0){
		perror("Nothing to deallocate");
		return 1;
	}

	// Look up memory by the key:
	vector<shared_mem_info>::iterator itr = SharedMemInfo.begin();
   shared_mem_info target;
	int key_found = 0;

	for(;itr != SharedMemInfo.end();itr++){
		if (itr->key == key){
			key_found = 1;
			// Deep copy values to target:
			target.shmid   = itr->shmid;
			target.shmaddr = itr->shmaddr;
			target.key     = itr->key;
			break;
		}
	}

	// If key not found nothing to do here. 
	// Notify user as an error (invalid key):
	if (!key_found){
		fprintf(stderr, "shared_mem:destroy_shm() key '%d' not found!", key);
		return 0;
	}

	// At this point we know there is a valid key and 
	// can detach all the segments from the vector:
	itr = SharedMemInfo.begin();
	for(;itr != SharedMemInfo.end();itr++){
		if(!detach_shm(itr->shmaddr)){
			// This means we must be done detaching
			// so just break out:
			break;
		}
	}

	// Create a shmid_ds see sys/shm.h man:
	struct shmid_ds shmid;

	// Mark segment to be destroyed with a call to shmctl applying IPC_RMID:

	if (shmctl(target.shmid,IPC_RMID, &shmid) != 0){
		// This can happen if multiple processes are started as the
		// memory can only be freed when the last process occupying
		// the memory detaches (see sys/shm.h)
		fprintf(stderr, 
				  "shared_mem:destroy_shm() can't remove memory segment at address 0x%p and IPC key %d Is the memory segment in use?\n",
				  target.shmaddr, target.key);
		return 0;
	}

	return 1;
}
