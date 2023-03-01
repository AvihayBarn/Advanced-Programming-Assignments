#include "codec.h"
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>
#include "thpool.h"

void task(void *arg){
	printf("Thread #%u working on %d\n", (int)pthread_self(), (int) arg);
}

int main(int argc, char *argv[])
{
	/*char data[] = "my text to encrypt";
	int key = 12;

	encrypt(data,key);
	printf("encripted data: %s\n",data);

	decrypt(data,key);
	printf("decripted data: %s\n",data);*/
	
		
	puts("Making threadpool with 4 threads");
	threadpool thpool = thpool_init(4);

	puts("Adding 40 tasks to threadpool");
	int i;
	for (i=0; i<40; i++){
		thpool_add_work(thpool, task, (void*)(uintptr_t)i);
	};

	thpool_wait(thpool);
	puts("Killing threadpool");
	thpool_destroy(thpool);

	return 0;
}
