#include "Buffer.h"

int main  () {
	// alokowanie pamięci współdzielonej
	segment_id = allocateSharedMemory(1);
	
	// inicjalizacja przyłączenia pamięci współdzielonej
	firstSharedMemoryAttachment();

	semaphore_id = semaphoreAllocation();

	// inicjalizacja semafora
	semaphoreInitialize(semaphore_id);

	return 0;
}