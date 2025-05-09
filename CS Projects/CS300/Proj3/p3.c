#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

int max_chairs;          // Number of chairs in the waiting room
int students;            // Number of students
int chairs_occupied = 0; // Number of chairs currently occupied
int recruiter_busy = 0;  // 1 if recruiter is interviewing, 0 if doing own work
int interviews_completed = 0; // Counter for completed interviews

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // Mutex for synchronization
sem_t student_sem; // Semaphore for students to wait for the recruiter
sem_t recruiter_sem; // Semaphore for recruiter to wait for students

void* producer(void* arg) {
    int id = *(int*)arg;

    while (1) {  // Keep trying until interviewed
        pthread_mutex_lock(&lock);

        if (chairs_occupied < max_chairs) { 
            // Student sits down
            chairs_occupied++;
            printf("Student %d: Sitting on a chair. Chairs occupied: %d\n", id, chairs_occupied);

            // Signal the recruiter that a student is waiting
            sem_post(&recruiter_sem);

            pthread_mutex_unlock(&lock);

            // Wait for the recruiter to call for the interview
            sem_wait(&student_sem);

            // Interview starts
            pthread_mutex_lock(&lock);
            recruiter_busy = 1;
            chairs_occupied--;
            printf("Student %d: Interview starting. Chairs occupied: %d\n", id, chairs_occupied);
            pthread_mutex_unlock(&lock);

            // Simulate interview time
            int interview_time = rand() % 5 + 1;
            sleep(interview_time);

            pthread_mutex_lock(&lock);
            recruiter_busy = 0;
            interviews_completed++;
            printf("Student %d: Finished interview. Total interviews completed: %d\n", id, interviews_completed);

            // If all students are interviewed, wake up recruiter to allow exit
            if (interviews_completed >= students) {
                sem_post(&recruiter_sem);  // Wake up recruiter to check exit condition
            }

            pthread_mutex_unlock(&lock);

            return NULL;  // Exit after interview
        }

        // No available chair, leave and return later
        printf("Student %d: All chairs occupied. Leaving and will return later.\n", id);
        pthread_mutex_unlock(&lock);

        // Simulate studying time before retrying
        int study_time = rand() % 10 + 1;
        sleep(study_time);
    }
}

void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);

        // Exit condition: If all students are interviewed, stop the loop
        if (interviews_completed >= students) {
            pthread_mutex_unlock(&lock);
            printf("Recruiter: All students have been interviewed. Exiting.\n");
            return NULL;  // Exit thread properly
        }

        pthread_mutex_unlock(&lock);

        // Wait for a student to arrive
        sem_wait(&recruiter_sem);

        pthread_mutex_lock(&lock);

        // Check again if all interviews are completed
        if (interviews_completed >= students) {
            pthread_mutex_unlock(&lock);
            printf("Recruiter: All students have been interviewed. Exiting.\n");
            return NULL;  // Exit properly
        }

        // Recruiter picks a student for an interview
        recruiter_busy = 1;
        printf("Recruiter is interviewing a student. Chairs occupied: %d\n", chairs_occupied);

        // Signal exactly one student to start the interview
        sem_post(&student_sem);
        pthread_mutex_unlock(&lock);

        // Simulate interview time
        int interview_time = rand() % 5 + 1;
        sleep(interview_time);

        pthread_mutex_lock(&lock);
        recruiter_busy = 0;
        printf("Recruiter finished interviewing. Total interviews completed: %d\n", interviews_completed);

        // If all interviews are complete, wake up recruiter one last time before exit
        if (interviews_completed >= students) {
            sem_post(&recruiter_sem);  // Wake up recruiter to check exit condition
            pthread_mutex_unlock(&lock);
            printf("Recruiter: All students have been interviewed. Exiting.\n");
            return NULL;  // Exit properly
        }

        pthread_mutex_unlock(&lock);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_chairs> <number_of_students>\n", argv[0]);
        exit(1);
    }

    max_chairs = atoi(argv[1]);
    students = atoi(argv[2]);

    srand(time(NULL));  // Seed random number generator

    // Initialize semaphores
    sem_init(&student_sem, 0, 0);      // Students wait for the recruiter
    sem_init(&recruiter_sem, 0, 0);    // Recruiter waits for students

    pthread_t recruiter_thread;
    pthread_t student_threads[students];
    int student_ids[students];

    // Create recruiter thread (consumer)
    pthread_create(&recruiter_thread, NULL, consumer, NULL);

    // Create student threads (producers)
    for (int i = 0; i < students; i++) {
        student_ids[i] = i + 1;
        pthread_create(&student_threads[i], NULL, producer, &student_ids[i]);
    }

    // Wait for all student threads to finish
    for (int i = 0; i < students; i++) {
        pthread_join(student_threads[i], NULL);
    }

    // Wait for the recruiter thread to finish
    pthread_join(recruiter_thread, NULL);

    // Destroy semaphores
    sem_destroy(&student_sem);
    sem_destroy(&recruiter_sem);

    printf("All interviews completed. Program exiting.\n");
    return 0;
}