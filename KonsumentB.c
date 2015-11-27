#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>

#include "Buffer.h"

int main  () {
	segment_id = allocateSharedMemory(0);

	semaphore_id = semaphoreAllocation();

/*	printf("KonsumentB#main: Shared Memory id: %d\n", segment_id);
	printf("KonsumentB#main: Semafor id: %d\n", semaphore_id);*/

	for (int i = 0; i < 10; ++i) {
		getElementInfB("B");
		usleep(10000);
	}

	printf("KONIEC KonsumentB\n");
	return 0;
}