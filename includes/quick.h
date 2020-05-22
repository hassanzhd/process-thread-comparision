#ifndef QUICK_H
#define QUICK_H
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

struct args {
    int *__array;
    int l;
    int h;
};

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

int partition(int __array[], int l, int h) {
    int pivot = __array[h];
    int j = l;

    for (int i = l; i < h; i++) {
        if (__array[i] <= pivot) {
            swap(&__array[i], &__array[j]);
            j++;
        }
    }
    swap(&__array[j], &__array[h]);
    return j;
}

void *qsThread(void *__args) {
    struct args *arg = (struct args *)__args;

    if (arg->l < arg->h) {
        int j = partition(arg->__array, arg->l, arg->h);
        struct args v1;
        struct args v2;
        v1.__array = arg->__array;
        v1.l = arg->l;
        v1.h = j - 1;

        v2.__array = arg->__array;
        v2.l = j + 1;
        v2.h = arg->h;

        pthread_t threads[THREAD_MAX];

        pthread_create(&threads[0], NULL, qsThread, &v1);
        pthread_create(&threads[1], NULL, qsThread, &v2);
        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
    }
    return NULL;
}

void quickSortThread() {
    printf("\n=====================\n");
    printf("SORTING USING THREAD\n");
    printf("=====================\n");
    struct args arg;
    arg.__array = quickTDataArray;
    arg.l = 0;
    arg.h = numberOfElements - 1;

    clock_t start;
    start = clock();

    qsThread(&arg);

    printf("\nElements after sorting:\n");
    for (int i = 0; i < numberOfElements; i++) {
        printf("%d ", quickTDataArray[i]);
    }
    printf("\n");

    start = clock() - start;
    printf("\nTime taken for sorting using threads: %f seconds\n", (float)start / CLOCKS_PER_SEC);
}

#endif