//Riley Fee
//12148611
//Project 3
//To compile use gcc p3test.c mytime.c -std=c99 -o p3
//To run use ./p3 chairs students left right

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include "mytime.h"

int max_chairs;
int students;
int chairs_occupied = 0;
int recruiter_busy = 0;
int interviews_completed = 0;
int left_time;
int right_time;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
sem_t student_sem;
sem_t recruiter_sem;

// Student thread function
void* student(void* arg) {
    int id = *(int*)arg;

    while (1) {  // Keep trying until interviewed
        pthread_mutex_lock(&lock);

        // Check if recruiter is available
        if (!recruiter_busy) {
            recruiter_busy = 1;
            pthread_mutex_unlock(&lock);

            printf("Student %d: Interview starting.\n", id);
            sleep(mytime(left_time, right_time));

            pthread_mutex_lock(&lock);
            interviews_completed++;
            recruiter_busy = 0;
            printf("Student %d: Finished interview. Total interviews completed: %d\n", id, interviews_completed);
            pthread_mutex_unlock(&lock);

            if (interviews_completed >= students) {
                sem_post(&recruiter_sem);
            }

            return NULL;
        }

        // If recruiter is busy, check for an available chair
        if (chairs_occupied < max_chairs) { 
            chairs_occupied++;
            printf("Student %d: Sitting on a chair. Chairs occupied: %d\n", id, chairs_occupied);

            sem_post(&recruiter_sem);
            pthread_mutex_unlock(&lock);

            sem_wait(&student_sem);

            // Interview starts
            pthread_mutex_lock(&lock);
            recruiter_busy = 1;
            chairs_occupied--;
            printf("Student %d: Interview starting. Chairs now occupied: %d\n", id, chairs_occupied);
            sleep(mytime(left_time, right_time));
            interviews_completed++;
            recruiter_busy = 0;
            printf("Student %d: Finished interview. Total interviews completed: %d\n", id, interviews_completed);
            pthread_mutex_unlock(&lock);

            if (interviews_completed >= students) {
                sem_post(&recruiter_sem);
            }

            return NULL;
        }

        // No available chair, leave and return later
        printf("Student %d: All chairs occupied. Leaving and will return later.\n", id);
        pthread_mutex_unlock(&lock);

        // Simulate studying time before retrying
        int study_time = mytime(left_time, right_time);
        sleep(study_time);
    }
}

void* recruiter(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);

        // Exit condition
        if (interviews_completed >= students) {
            pthread_mutex_unlock(&lock);
            printf("Recruiter: All students have been interviewed. Exiting.\n");
            return NULL;
        }

        pthread_mutex_unlock(&lock);

        // Wait for a student to arrive
        sem_wait(&recruiter_sem);

        pthread_mutex_lock(&lock);

        // Recruiter picks a student for interview
        recruiter_busy = 1;

        // Signal student to start interview
        sem_post(&student_sem);
        pthread_mutex_unlock(&lock);

        sleep(mytime(left_time, right_time));

        pthread_mutex_lock(&lock);
        recruiter_busy = 0;

        // If all interviews are complete, wake up recruiter to exit recruiter thread
        if (interviews_completed >= students) {
            sem_post(&recruiter_sem);
            pthread_mutex_unlock(&lock);
            printf("Recruiter: All students have been interviewed. Exiting.\n");
            return NULL;
        }

        pthread_mutex_unlock(&lock);
    }
}

int main(int argc, char* argv[]) {
    max_chairs = atoi(argv[1]);
    students = atoi(argv[2]);
    left_time = atoi(argv[3]);
    right_time = atoi(argv[4]);

    sem_init(&student_sem, 0, 0);
    sem_init(&recruiter_sem, 0, 0);

    pthread_t recruiter_thread;
    pthread_t student_threads[students];
    int student_ids[students];

    pthread_create(&recruiter_thread, NULL, recruiter, NULL);

    for (int i = 0; i < students; i++) {
        student_ids[i] = i + 1;
        pthread_create(&student_threads[i], NULL, student, &student_ids[i]);
    }

    for (int i = 0; i < students; i++) {
        pthread_join(student_threads[i], NULL);
    }

    pthread_join(recruiter_thread, NULL);

    sem_destroy(&student_sem);
    sem_destroy(&recruiter_sem);
    return 0;
}