/**
 * Rendezvous
 *
 * Two threads executing chunks of work in a lock step - skeleton
 * code.
 *
 * Author: Nikos Nikoleris <nikos.nikoleris@it.uu.se>
 *
 */

#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* abort(), [s]rand() */
#include <unistd.h>    /* sleep() */
#include <semaphore.h> /* sem_...() */
#include <pthread.h>   /* pthread_...() */

#define LOOPS 5
#define NTHREADS 3
#define MAX_SLEEP_TIME 3


/* TODO: Make the two threads perform their iterations in a
 * predictable way. Both should perform iteration 1 before iteration 2
 * and then 2 before 3 etc. */

sem_t mutex_A, mutex_B;

void *
threadA(void *param __attribute__((unused)))
{
    int i;
    
    for (i = 0; i < LOOPS; i++) {

        sem_wait(&mutex_A);
        printf("threadA --> %d iteration\n", i);
      
        sem_post(&mutex_B);
      
    }

    pthread_exit(0);
}


void *
threadB(void *param  __attribute__((unused)))
{
    int i;

    for (i = 0; i < LOOPS; i++) {

        sem_wait(&mutex_B);
        printf("threadB --> %d iteration\n", i);
   
        sem_post(&mutex_A);
      
    }

    pthread_exit(0);
}

int
main()
{
    pthread_t tidA, tidB;

    srand(time(NULL));
    pthread_setconcurrency(3);

    if (sem_init(&mutex_A, 0, 1) != 0 || sem_init(&mutex_B, 0, 0) != 0) {
        perror("Failed to init the semaphore\n");
    }
    
    for (int i = 0; i < 100; i++) {
        
    
        if (pthread_create(&tidA, NULL, threadA, NULL) ||
            pthread_create(&tidB, NULL, threadB, NULL)) {
            perror("pthread_create");
            abort();
        }
        if (pthread_join(tidA, NULL) != 0 ||
            pthread_join(tidB, NULL) != 0) {
            perror("pthread_join");
            abort();
        }
    }

    sem_destroy(&mutex_A);
    sem_destroy(&mutex_B);

    return 0;
}

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * c-file-style: "stroustrup"
 * End:
 */
