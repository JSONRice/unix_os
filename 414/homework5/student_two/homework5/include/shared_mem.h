#ifndef _SHARED_MEM_H_
#define _SHARED_MEM_H_

#define MAXNUM     10
#define MAXELEMENT 24
#define OK         0
#define ERROR      -1


typedef struct id {
    int key;
    int shmid;
    void *address;
} ID;

void *connect_shm(int key, int size);
int detach_shm(void *address);
int destroy_shm(int key);
int findIndex(int key);
 
#endif
