// 
// homework 4, Unix Systems Programming
// Fall
//
// file: shmlib.h
// This is the header for shared_mem.c which gets built as shmlib.a.  
// Three functions are contained: for connecting, detaching, and 
// destroying shared memory.  Read below for more info.

#define MAX_SHM 20
//
// this is the maximum number of shared memory segments allowed in this
// library.  (if you need more, this number can be increased, so long as
// the library is then recompiled.)

void* connect_shm (int key, int size);
//
// key: a system wide identifier for the shared memory. pick 
// a number that no other program needs.  for two programs 
// to share memory, they should each use the same key.
// size: the size in bytes of the shared segment to create.
// returns: a pointer to the shared memory that has been
// attached (and created if need be).  Upon failure, NULL is 
// returned.

int detach_shm (void* addr);
//
// addr: the address of the shared memory to be detached.  This 
// should be the same as the return value of the repective 
// connect_shm call.
// returns: OK or ERROR as defined in "errcodes.h"

int destroy_shm (int key);
// 
// key: same value as passed to connect_shm.  All memory that was
// attached through connect_shm with this key will be detached.
// then the memory will be deleted from the system.
// returns: OK or ERROR as defined in "errcodes.h"

