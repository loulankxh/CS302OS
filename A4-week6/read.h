#include"init.h"

void *reader(int *buffer){
    // Reader should read the value of buffer
    // Reader cannot start to work when there is any writer working
    // You should implement this using semaphore
    sem_wait(&rc);
    readcount = readcount + 1;
    if(readcount == 1){	// guarantee it as the first reader
    	sem_wait(&db);
    }
    // else: there is one reader already get lock db and thus no writer can write
    // You should output this just after this thread gets semaphore
    printf("Reader gets sem\n");
    sem_post(&rc);

    // you should output this just before this thread releases the semaphore
    printf("Reader reads %d and releases\n", *buffer); 
    sem_wait(&rc);
    readcount = readcount - 1;
    if(readcount == 0){ // guarantee it as the last reader
        sem_post(&db);
    }
    sem_post(&rc);
}

