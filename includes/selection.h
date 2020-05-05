#ifndef SELECTION_H
#define SELECTION_H
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "data.h"
#define THREAD_MAX 4  // maximum number of threads depending upon cores

int threadNumber = 0;  // helps in partitioning array (start/end values)
int *arr;              /* for minimum calculation as no inner loop exists hence array must be 
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
        if (arr[i] < arr[min]) {
            min = i;
        }
    }
}

void selectionSortThread() {
    pthread_t threads[THREAD_MAX];
    clock_t start;
    start = clock();

    arr = selectionArray;
    boundary = numberOfElements;
    for (int i = 0; i < numberOfElements; i++) {
        for (int i = 0; i < THREAD_MAX; i++)
            // creating THREAD-MAX number of threads
            pthread_create(&threads[i], NULL, minimumThread, (void *)NULL); /* 
                In selection sort costy operation is finding index of minimum values hence we
                used minimumThread() inside pthread_create()
             */

        // joining all THREAD_MAX threads
        for (int i = 0; i < THREAD_MAX; i++)
            pthread_join(threads[i], NULL);

        int temp = *(selectionArray + i);
        selectionArray[i] = arr[min];
        arr[min] = temp;

        // updating/ resetting values
        threadNumber = 0;
        arr++;
        boundary--;
        min = 0;
    }

    printf("\nElements after sorting:\n");
    for (int i = 0; i < numberOfElements; i++) {
        printf("%d ", selectionArray[i]);
    }
    printf("\n");

    start = clock() - start;
    printf("\nTime taken for sorting: %f seconds\n", (float)start / CLOCKS_PER_SEC);
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
            if (selectionArray[j] < selectionArray[min]) {
                min = j;
            }
        }

        temp = selectionArray[i];
        selectionArray[i] = selectionArray[min];
        selectionArray[min] = temp;
    }

    printf("\nArray after sort:\n");

    for (int i = 0; i < numberOfElements; i++) {
        printf("%d ", selectionArray[i]);
    }
    printf("\n");

    start = clock() - start;
    printf("Time taken for searching: %f seconds\n", (float)start / CLOCKS_PER_SEC);
 */

#endif