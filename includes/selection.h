#ifndef SELECTION_H
#define SELECTION_H
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "data.h"

int threadNumber = 0;

void minimumThread() {}

void selectionSortThread() {
    clock_t start;
    start = clock();

    for (int i = 0; i < numberOfElements; i++) {
        int temp = 0;
        int min = i;

        for (int j = i + 1; j < numberOfElements; j++) {
            if (array[j] < array[min]) {
                min = j;
            }
        }

        temp = array[i];
        array[i] = array[min];
        array[min] = temp;
    }

    printf("\nArray after sort:\n");

    for (int i = 0; i < numberOfElements; i++) {
        printf("%d\n", array[i]);
    }

    start = clock() - start;
    printf("Time taken for searching: %f seconds\n", (float)start / CLOCKS_PER_SEC);
}

/* 
    clock_t start;
    start = clock();
    start = clock() - start;
    printf("Time taken for searching: %f seconds\n", (float)start / CLOCKS_PER_SEC);
*/

#endif