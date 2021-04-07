#include"init.h"

void *writer(int *buffer){
    // Writer should change buffer to buffer+1 (e.g., before *buffer==0, after *buffer==1)
    // Writer cannot start to work when there is any reader or writer working
    // You should implement this using semaphore
    sem_wait(&db); // get lock
    // You should output this just after this thread gets semaphore
    printf("writer gets sem\n");
    (*buffer) = (*buffer) + 1; // write
    // you should output this just before this thread releases the semaphore
    printf("writer writes %d and releases\n", *buffer);
    sem_post(&db); // release lock
}

