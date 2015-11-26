#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include "Buffer.h"

int main  () {
	segment_id = shmget (segment_key, shared_segment_size, 
							IPC_EXCL | S_IRUSR | S_IWUSR);

	printf("Id segmentu: %d\n", segment_id);

	// Attochment (przyłączenie)
	shared_memory = (char*) shmat (segment_id, (void*) 0x5000000, 0);
	printf ("shared memory reattached at address %p\n", shared_memory);
	printf ("%s\n", shared_memory);

	// Detach (odłączenie)
	shmdt (shared_memory);

	// Deallocate (likwidacja)
	shmctl (segment_id, IPC_RMID, 0);

	return 0;
}