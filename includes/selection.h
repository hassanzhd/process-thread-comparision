#ifndef SELECTION_H
#define SELECTION_H
#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "data.h"

/* Selection sort thread implementation */
#define THREAD_MAX 4  // maximum number of threads depending upon cores

int threadNumber = 0;  // helps in partitioning array (start/end values)
int *threadArr;        /* for minimum calculation as no inner loop exists hence array must be 
                        in sync with main loop. Increments with every iteration */
int boundary = 0;      /* helps in new array size calculation with every iteration  */
int min = 0;           /* index of minimum value */

void *minimumThread(void *__args) {
    int i, start = threadNumber++;
    float factor = ceil((float)boundary / THREAD_MAX); /* 
        factor is used because with array size decreasing and partitioning being done, invalid 
        number can occur in start/ end values. 
     */
    for (i = start * (factor); (i < (start + 1) * factor) && i < boundary; i++) {
        if (threadArr[i] < threadArr[min]) {
            min = i;
        }
    }
}

void selectionSortThread() {
    pthread_t threads[THREAD_MAX];

    printf("\n=====================\n");
    printf("SORTING USING THREAD\n");
    printf("=====================\n");

    clock_t start;
    start = clock();

    threadArr = selectionTDataArray;
    boundary = numberOfElements;
    for (int i = 0; i < numberOfElements; i++) {
        for (int i = 0; i < THREAD_MAX; i++) {
            // creating THREAD-MAX number of threads
            pthread_create(&threads[i], NULL, minimumThread, (void *)NULL); /* 
                In selection sort costy operation is finding index of minimum values hence we
                used minimumThread() inside pthread_create()
             */
        }
        // joining all THREAD_MAX threads
        for (int i = 0; i < THREAD_MAX; i++) {
            pthread_join(threads[i], NULL);
        }
        int temp = *(selectionTDataArray + i);
        selectionTDataArray[i] = threadArr[min];
        threadArr[min] = temp;

        // updating/ resetting values
        threadNumber = 0;
        threadArr++;
        boundary--;
        min = 0;
    }

    printf("\nElements after sorting:\n");
    for (int i = 0; i < numberOfElements; i++) {
        printf("%d ", selectionTDataArray[i]);
    }
    printf("\n");

    start = clock() - start;
    printf("\nTime taken for sorting using threads: %f seconds\n", (float)start / CLOCKS_PER_SEC);
}

/* Selection sort process implementation */
#define PROCESS_MAX 4
int processNumber;
int *processArr;
int fd[4][2];

void minimumProcess() {
    int i, start = processNumber;
    float factor = ceil((float)boundary / PROCESS_MAX); /* 
        factor is used because with array size decreasing and partitioning being done, invalid 
        number can occur in start/ end values. 
     */
    for (i = start * (factor); (i < (start + 1) * factor) && i < boundary; i++) {
        if (processArr[i] < processArr[min]) {
            min = i;
        }
    }
}

void IPC() {
    pid_t process[4];
    int minArray[4];
    process[0] = fork();
    if (process[0] == 0) {
        minimumProcess();
        write(fd[0][1], &min, sizeof(int));
        exit(0);
    } else if (process[0] > 0) {
        process[1] = fork();
        processNumber++;
        if (process[1] == 0) {
            minimumProcess();
            write(fd[1][1], &min, sizeof(int));
            exit(0);
        } else if (process[1] > 0) {
            process[2] = fork();
            processNumber++;
            if (process[2] == 0) {
                minimumProcess();
                write(fd[2][1], &min, sizeof(int));
                exit(0);
            } else if (process[2] > 0) {
                process[3] = fork();
                processNumber++;
                if (process[3] == 0) {
                    minimumProcess();
                    write(fd[3][1], &min, sizeof(int));
                    exit(0);
                } else if (process[3] > 0) {
                    wait(NULL);
                }
                wait(NULL);
            }
            wait(NULL);
        }

        wait(NULL);
        for (int i = 0; i < PROCESS_MAX; i++) {
            read(fd[i][0], &minArray[i], sizeof(int));
        }

        min = minArray[0];
        for (int i = 1; i < PROCESS_MAX; i++) {
            if (processArr[minArray[i]] < processArr[min]) {
                min = minArray[i];
            }
        }
    }
}

void selectionSortProcess() {
    processNumber = min = 0;
    boundary = numberOfElements;
    processArr = selectionPDataArray;

    printf("\n=====================\n");
    printf("SORTING USING PROCESS\n");
    printf("=====================\n");

    clock_t start;
    start = clock();

    for (int i = 0; i < 4; i++) {
        pipe(fd[i]);
    }

    for (int i = 0; i < numberOfElements; i++) {
        IPC();

        int temp = *(selectionPDataArray + i);
        selectionPDataArray[i] = processArr[min];
        processArr[min] = temp;

        processNumber = 0;
        processArr++;
        boundary--;
        min = 0;
    }
    printf("\nElements after sorting:\n");
    for (int i = 0; i < numberOfElements; i++) {
        printf("%d ", selectionPDataArray[i]);
    }
    printf("\n");

    start = clock() - start;
    printf("Time taken for sorting using processes: %f seconds\n", (float)start / CLOCKS_PER_SEC);
}

/* 
    clock_t start;
    start = clock();
    start = clock() - start;
    printf("Time taken for searching: %f seconds\n", (float)start / CLOCKS_PER_SEC);
*/

/* clock_t start;
    start = clock();

    for (int i = 0; i < numberOfElements; i++) {
        int temp = 0;
        int min = i;

        for (int j = i + 1; j < numberOfElements; j++) {
            if (selectionTDataArray[j] < selectionTDataArray[min]) {
                min = j;
            }
        }

        temp = selectionTDataArray[i];
        selectionTDataArray[i] = selectionTDataArray[min];
        selectionTDataArray[min] = temp;
    }

    printf("\nArray after sort:\n");

    for (int i = 0; i < numberOfElements; i++) {
        printf("%d ", selectionTDataArray[i]);
    }
    printf("\n");

    start = clock() - start;
    printf("Time taken for searching: %f seconds\n", (float)start / CLOCKS_PER_SEC);
 */

#endif