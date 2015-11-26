#include "Buffer.h"

int main  () {
	// alokowanie pamięci współdzielonej
	segment_id = allocateSharedMemory(0);
	
	// alokowanie semafora
	semaphore_id = semaphoreAllocation();

	printf("ProducentB#main: Shared Memory id: %d\n", segment_id);
	printf("ProducentB#main: Semafor id: %d\n", semaphore_id);

	for(int i = 0; i < 20; ++i) {
		setElement("B");

		usleep(20);
	}
	
	return 0;
}