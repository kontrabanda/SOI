#include "Buffer.h"

int main () {
	segment_id = allocateSharedMemory(0);

	// Deallocate (likwidacja) pamięci współdzielonej
	shmctl (segment_id, IPC_RMID, 0);

	semaphore_id = semaphoreAllocation();

	semaphoreDeallocate(semaphore_id);

	printf("KONIEC\n");

	return 0;
}