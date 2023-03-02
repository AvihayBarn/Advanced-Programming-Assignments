#include "codec.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define THREAD_NUM 4 // c

int key;
char *flag;
// char arr[256][BUFFER_SIZE];

typedef struct Task{
	int index;
	char buffer[BUFFER_SIZE];
} Task;

Task taskQueue[256];
int taskCount = 0;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

void executeTask(Task* task) {
	if (strcmp(flag, "-e") == 0){
		encrypt(task->buffer, key);
		printf("%s", task->buffer);
		// memcpy(&arr[task->index], task->buffer, sizeof(task->buffer));
	}
	else if (strcmp(flag, "-d") == 0){
		decrypt(task->buffer, key);
		printf("%s", task->buffer);
		// memcpy(&arr[task->index], task->buffer, sizeof(task->buffer));
	}
}

void submitTask(Task task) {
    pthread_mutex_lock(&mutexQueue);
    taskQueue[taskCount] = task;
    taskCount++;
    pthread_mutex_unlock(&mutexQueue);
    pthread_cond_signal(&condQueue);
}

void* startThread(void* args) {
    while (taskCount > 0){
		Task task;

		pthread_mutex_lock(&mutexQueue);
		while (taskCount == 0)
			pthread_cond_wait(&condQueue, &mutexQueue);

		task = taskQueue[0];
		for (int i = 0; i < taskCount - 1; i++)
			taskQueue[i] = taskQueue[i + 1];
		taskCount--;
		pthread_mutex_unlock(&mutexQueue);
		executeTask(&task);
	}
	return 0;
}

int main(int argc, char* argv[]) {
    // Check valid input
	if (argc != 3)
		exit(0);

	// Get data from input args
	key = atoi(argv[1]);
	flag = argv[2];

	if (strcmp(flag, "-e") != 0 && strcmp(flag, "-d") != 0)
		exit(0);
	

	pthread_t threads[THREAD_NUM];
	pthread_mutex_init(&mutexQueue, NULL);
	pthread_cond_init(&condQueue, NULL);

	// Read file from the input
	char buffer[BUFFER_SIZE]; // Buffer to store data
	int size = read(0, buffer, BUFFER_SIZE);
	Task newTask;
	int index = taskCount + 1; 
	newTask.index = index;
	memset(&newTask.buffer,0,BUFFER_SIZE);
	memcpy(&newTask.buffer, buffer, size);

	submitTask(newTask);
	
	// Create threads
	for (int i = 0; i < THREAD_NUM; i++)
		if (pthread_create(&threads[i], NULL, &startThread, NULL) != 0)
			perror("Failed to create the thread");

	for (int i = 0; i < THREAD_NUM; i++)
		if (pthread_join(threads[i], NULL) != 0)	
			perror("Failed to join the thread");

			
	pthread_cond_destroy(&condQueue);
	pthread_mutex_destroy(&mutexQueue);
	// printf("%s", arr[index]);


	return 0;
}
