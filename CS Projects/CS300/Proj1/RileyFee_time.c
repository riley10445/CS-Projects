// RILEY FEE CWID 12148611

#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/mman.h>

#define SHARED_MEMORY_NAME "/shared"
#define SHARED_MEMORY_SIZE sizeof(struct timeval)

int main(int argc, char *argv[]) {
    //CREATE AND FORMAT SHARED MEMORY
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(struct timeval));
    struct timeval *shared_time = mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

pid_t pid;
    pid = fork();
    if(pid < 0){ //ERROR
        printf("An error has occurred, exiting");
        exit(1);
    }
    else if(pid == 0){ //CHILD
        gettimeofday(shared_time, NULL);    
        execvp(argv[1], &argv[1]);  
    } 
    else{ //PARENT
        wait(NULL);
        struct timeval end;
        gettimeofday(&end, NULL);
        double total_time = (double)(end.tv_sec - shared_time->tv_sec) 
                          + (double)(end.tv_usec - shared_time->tv_usec)/1000000;
        printf("Parent PID: %d\nChild PID: %d\nElapsed time: %f\n", (int) getpid(), pid, total_time);
        shm_unlink("/shared");
    }
    return 0;     
}