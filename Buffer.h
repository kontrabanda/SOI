#ifndef BUFFER_FILE
#define BUFFER_FILE

#include <string.h>
//#include <conio.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/* zmienne i stałe pamięci współdzielonej */
extern const key_t segment_key ;
char* shared_memory;
int segment_id;
struct shmid_ds shmbuffer;
int segment_size;
extern const int shared_segment_size;

int allocateSharedMemory ();
void firstSharedMemoryAttachment ();


/* zmienne i stałe semaforów */
enum {
	MUTEX,
	FULL_BUFFER,
	EMPTY_BUFFER,
	INF_A,
	INF_B
};

extern const key_t SEMAPHORE_ID;
extern const int BufferSize;

int semaphore_id;

int semaphoreAllocation ();
int semaphoreDeallocate (int semid) ;
int semaphoreInitialize (int semid);
int semaphoreWait (int semid, int type);
int sempahorePost (int semid, int type);

int setElement (const char element[]);
int getElement (const char name[]);

int lock (int semid);
int unlock (int semid);

#endif