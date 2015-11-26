#include "Buffer.h"

int main  () {
	// alokowanie pamięci współdzielonej
	segment_id = allocateSharedMemory(0);

	// alokowanie semafora
	semaphore_id = semaphoreAllocation();

	printf("ProducentA#main: Shared Memory id: %d\n", segment_id);
	printf("ProducentA#main: Semafor id: %d\n", semaphore_id);

	for(int i = 0; i < 10; ++i) {
		setElement("A");

		usleep(10000);
	}

	return 0;
}