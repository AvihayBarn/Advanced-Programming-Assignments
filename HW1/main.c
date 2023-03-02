#include "codec.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "thpool.h"

#define BUFFER 1024
#define THREADS 4

 
typedef struct Package{
	int key;
	char flag[2];
	
} Package;


void WritingToFile(void *arg){

	Package* package = (Package*)arg;
	char buffer[BUFFER];
	memset(buffer,0,BUFFER);
	int size = read(0,buffer,BUFFER);
	if (strcmp(package->flag , "-e") == 0){
		encrypt(buffer, package->key);
		printf("%s", buffer);
		
	}
	else if (strcmp(package->flag , "-d") == 0){
		decrypt(buffer, package->key);
		printf("%s", buffer);
		
	}
}

int main(int argc, char *argv[])
{
	
	/*char data[] = "my text to encrypt";
	int key = 12;

	encrypt(data,key);
	printf("encripted data: %s\n",data);

	decrypt(data,key);
	printf("decripted data: %s\n",data);*/
	
	if(argc != 3)
	{
		perror("Not a valid number of arguments");
		exit(0);
	}
	struct Package* package = malloc( sizeof( struct Package));
	package->key = atoi(argv[1]);

	if(!(strcmp(argv[2],"-e") == 0)  || !(strcmp(argv[2],"-d") == 0) )
	{
		perror("Not a valid flag");
		exit(0);
	}
	package->flag = (strcmp(argv[2], "-e")) ? "-e" : "-d";
		
	puts("Making threadpool with 4 threads");
	threadpool thpool = thpool_init(4);

	puts("Adding 4 tasks to threadpool");
	
    for (int i=0; i < THREADS; i++){
		thpool_add_work(thpool, WritingToFile, (void*)(Package*)package);
	};
	
	

	thpool_wait(thpool);
	puts("Killing threadpool");
	thpool_destroy(thpool);
	free(package);

	return 0;
}
