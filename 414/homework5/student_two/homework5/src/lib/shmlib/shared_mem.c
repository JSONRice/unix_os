#include <sys/types.h>
#include <sys/param.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "shared_mem.h"

/*
 * Name: Student 2
 * Proj: HW #4
 * Date: 
 * Desc: Library that provides mechanism to attach, detach and 
 *       destroy shared memory.
*/

ID keyData[MAXNUM];
int num = 0;

void * connect_shm(int key, int size) {
    int shmid;
    void * address;
    int index;

    shmid = shmget(key, size, IPC_CREAT | 0644);

    if (shmid < 0) {
        perror("shmget");
        printf("error exiting\n");
        exit(1);
    }

    /* store information about the shared memory into an array.
       This allows for multiple shared memory to attach
     */
    index = findIndex(key);
    if (index == -1) {
        address = (void *) shmat(shmid, NULL, 0);
 
        keyData[num].key = key;
        keyData[num].shmid = shmid;
        keyData[num].address = address;
        num++;
    }
    else { 
        address = keyData[index].address;
    }

    return address;
}

/* 
 * find the index of the shared memory with passin key
 * inside the global array
*/
int findIndex(int key) {
    int i;

    for (i = 0; i < num+1; i++) {
        if (keyData[i].key == key)
            return i;
    }
    return -1;
}

/* detach the shared memory with the passed in address */
int detach_shm(void *address) {
    if (shmdt(address) == 0) {
        return OK;
    }
    return ERROR;
}

/* detroy the shared memory with the passed in key */
int destroy_shm(int key) {
    int index;
    int shmid;

    /* see if there is memory assicated with the key */
    index = findIndex(key);
    if (index == -1) {
        return ERROR;
    }
    else { 
        shmid = keyData[index].shmid;        
    }
    if (shmctl(shmid, IPC_RMID, NULL) == 0) {
        return OK;
    }
    return ERROR;
}
     
