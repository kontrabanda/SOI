#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include "Buffer.h"

int main () {
	segment_id = allocateSharedMemory(0);

	semaphore_id = semaphoreAllocation();

	printf("KonsumentA#main: Shared Memory id: %d\n", segment_id);
	printf("KonsumentA#main: Semafor id: %d\n", semaphore_id);

	for (int i = 0; i < 20; ++i) {
		getElement("A");
		usleep(30000);
	}

	printf("KONIEC KonsumentA\n");
	return 0;
}