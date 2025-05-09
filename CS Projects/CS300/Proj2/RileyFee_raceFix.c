/*
Riley Fee
12148611
Project 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "common.h"
#include "common_threads.h"

int max;
volatile int counter = 0;
pthread_mutex_t lock; 

void *mythread(void *arg) {
    char *letter = arg;
    int i;
    printf("%s: begin [addr of i: %p,] [addr of counter: %p]\n", letter, &i,(unsigned int) &counter);
    pthread_mutex_lock(&lock);
    for (i = 0; i < max * 1000; i++) {
	counter = counter + 1;
    }
    pthread_mutex_unlock(&lock);
    printf("%s: done\n", letter);
    return NULL;
}
                                                                             
int main(int argc, char *argv[]) {                    
    if (argc != 2) {
	fprintf(stderr, "usage: main-first <loopcount>\n");
	exit(1);
    }
    max = atoi(argv[1]);

    pthread_mutex_init(&lock, NULL);
    pthread_t p1, p2;
    printf("main: begin [counter = %d] [%x]\n", counter, 
	   (unsigned int) &counter);
    Pthread_create(&p1, NULL, mythread, "A"); 
    Pthread_create(&p2, NULL, mythread, "B");
    Pthread_join(p1, NULL); 
    Pthread_join(p2, NULL); 
    pthread_mutex_destroy(&lock); 
    printf("main: done\n [counter: %d]\n [should: %d]\n", 
	   counter, max*2*1000);

    return 0;
}