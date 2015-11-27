#include "Buffer.h"
#include <errno.h>

const key_t segment_key = 1234;
const int shared_segment_size = 1000;

const key_t SEMAPHORE_ID = 285;
const int SEMAPHORE_COUNT = 6;

const int BufferSize = 9;

const int INIF_A_BUFFER = 5;
const int INIF_B_BUFFER = 3;

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
	struct seminfo *__buf;
};

int semaphoreAllocation () {
	return semget(SEMAPHORE_ID, SEMAPHORE_COUNT, 0777 | IPC_CREAT);
}

int semaphoreDeallocate (int semid) {
	union semun ignored_argument;
	return semctl(semid, 1, IPC_RMID, ignored_argument);
}

int semaphoreInitialize (int semid) {
	int result = 0;

	if(semctl(semid, MUTEX, SETVAL, 1) != -1
		&& semctl(semid, FULL_BUFFER, SETVAL, 0) != -1
		&& semctl(semid, EMPTY_BUFFER, SETVAL, BufferSize) != -1
		&& semctl(semid, INF_A, SETVAL, 0) != -1
		&& semctl(semid, INF_B, SETVAL, 0) != -1
		&& semctl(semid, MUTEX_ADDITIONAL, SETVAL, 0) != -1) {

		result = 1;
	}

	return result;
}

int semaphoreWait (int semid, int type) {
	struct sembuf operations = {0, 0, 0};

	operations.sem_num = type;
	operations.sem_op = -1;
	// operations.sem_flg = SEM_UNDO;

	return semop (semid, &operations, 1);
}

int semaphoreDecrement (int semid, int type) {
	struct sembuf operations = {0, 0, 0};

	operations.sem_num = type;
	operations.sem_op = -1;
	operations.sem_flg = IPC_NOWAIT;

	return semop (semid, &operations, 1);
}

int sempahorePost (int semid, int type) {
	struct sembuf operations = {0, 0, 0};

	operations.sem_num = type;
	operations.sem_op = 1;
	// operations.sem_flg = SEM_UNDO;

	return semop (semid, &operations, 1);
}

int lock (int semid, int semType) {
	return semaphoreWait(semid, semType);
}

int unlock (int semid, int semType) {
	return sempahorePost(semid, semType);
}

int allocateSharedMemory (int checkedFirstTime) {
	// Allocate (utworzenie)
	if (checkedFirstTime == 1) {
		return shmget (segment_key, shared_segment_size, 
						IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	} else {
		return shmget (segment_key, shared_segment_size, 
						IPC_EXCL | S_IRUSR | S_IWUSR);
	}
}

void firstSharedMemoryAttachment () {
	// Attachment (przyłączenie)
	shared_memory = (char*) shmat (segment_id, 0, 0);
	
	shmctl (segment_id, IPC_STAT, &shmbuffer);
	segment_size = shmbuffer.shm_segsz;

	// Detachment (odłączenie)
	shmdt (shared_memory);
}

int setElement (const char element[]) {
	int result = 0;

	// zwieksz semafor pusty (dodaj nowy element)
	semaphoreWait(semaphore_id, EMPTY_BUFFER);

	// zablokowanie zapisu (mutex)
	if (lock(semaphore_id, MUTEX) == -1) {return 1;}
	else { printf("LOCK\n"); }

	shared_memory = (char*) shmat (segment_id, 0, 0);

	sprintf (shared_memory + strlen(shared_memory), element);

	result = strlen(shared_memory);

	printf("Producent%s, BUFFER: %s, BUFFER_SIZE: %d\n", element, shared_memory, strlen(shared_memory));
	shmdt (shared_memory);

	
	// odblokowanie zapisu mutex
	if (unlock(semaphore_id, MUTEX) == -1) {return 1;}
	else { printf("UNLOCK\n"); }

	sempahorePost(semaphore_id, FULL_BUFFER);

	return result;
}

int getElement (const char name[]) {
	// zwieksz semafor pusty (dodaj nowy element)
	semaphoreWait(semaphore_id, FULL_BUFFER);

	// zablokowanie zapisu (mutex)
	if (lock(semaphore_id, MUTEX) == -1) {return -1;}
	else { printf("LOCK\n"); }

	shared_memory = (char*) shmat (segment_id, 0, 0);
	
	const int N = strlen(shared_memory);
	char BufferCopy[BufferSize];

	for (int i = 1; i < N; ++i) {
		BufferCopy[i-1] = shared_memory[i];
	}

	for (int i = 0; i < N-1; ++i) {
		shared_memory[i] = BufferCopy[i];
	}
	shared_memory[N-1] = '\0';

	printf("Konsument%s, BUFFER: %s, BUFFER_SIZE: %d\n", name, shared_memory, strlen(shared_memory));

	shmdt(shared_memory);

	// odblokowanie zapisu mutex
	if (unlock(semaphore_id, MUTEX) == -1) {return -1;}
	else { printf("UNLOCK\n");}

	sempahorePost(semaphore_id, EMPTY_BUFFER);

	return 0;
}

void getElementInfA (const char name[]) {
	//lock(semaphore_id, MUTEX_ADDITIONAL);

	semaphoreWait(semaphore_id, INF_A);
	semaphoreDecrement(semaphore_id, INF_B);
	
	//unlock(semaphore_id, MUTEX_ADDITIONAL);

	getElement(name);
}

void getElementInfB (const char name[]) {
	//lock(semaphore_id, MUTEX_ADDITIONAL);

	semaphoreWait(semaphore_id, INF_B);
	semaphoreDecrement(semaphore_id, INF_A);
	
	//unlock(semaphore_id, MUTEX_ADDITIONAL);

	getElement(name);
}

void setElementWrapper (const char name[]) {
	int bufferSize = setElement(name);

	if (bufferSize >= INIF_A_BUFFER) {
		printf("SET A BUFFER SIZE CASE\n");
		sempahorePost(semaphore_id, INF_A);
	} else {
		// semctl(semaphore_id, INF_A, SETVAL, 0);
	}

	if (bufferSize >= INIF_B_BUFFER) {
		printf("SET B BUFFER SIZE CASE\n");
		sempahorePost(semaphore_id, INF_B);
	} else {
		// semctl(semaphore_id, INF_B, SETVAL, 0);
	}
}