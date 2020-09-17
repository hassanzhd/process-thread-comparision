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

#define THREAD_MAX 4   // maximum number of threads depending upon cores
#define PROCESS_MAX 4  // maximum number of processes depending upon cores

int qfd[PROCESS_MAX][2];             // file discriptors used for pipes
int qSizeArray[PROCESS_MAX] = {-1};  // size of different partitions (i.e 4 different divisions)

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

        int firstThread = pthread_create(&threads[0], NULL, qsThread, &v1);
        int secondThread = pthread_create(&threads[1], NULL, qsThread, &v2);

        if (firstThread == 0) {
            pthread_join(threads[0], NULL);
        }

        if (secondThread == 0) {
            pthread_join(threads[1], NULL);
        }
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

    // printf("\nElements after sorting:\n");
    // for (int i = 0; i < numberOfElements; i++) {
    //     printf("%d ", quickTDataArray[i]);
    // }
    // printf("\n");

    start = clock() - start;
    printf("\nTime taken for sorting using threads: %f seconds\n", (float)start / CLOCKS_PER_SEC);
}

void quickSort(int __array[], int l, int h) {
    if (l < h) {
        int j = partition(__array, l, h);
        quickSort(__array, l, j - 1);
        quickSort(__array, j + 1, h);
    }
}

void quickMerge(int __original[], int __left[], int __right[], int __llength, int __rlength) {
    int i = 0, j = 0, k = 0;
    while (i < __llength && j < __rlength) {
        if (__left[i] <= __right[j]) {
            __original[k] = __left[i];
            k++;
            i++;
        } else {
            __original[k] = __right[j];
            k++;
            j++;
        }
    }

    while (i < __llength) {
        __original[k] = __left[i];
        i++;
        k++;
    }

    while (j < __rlength) {
        __original[k] = __right[j];
        j++;
        k++;
    }
}

void quickSortProcess() {
    printf("\n=====================\n");
    printf("SORTING USING PROCESS\n");
    printf("=====================\n");

    int process[PROCESS_MAX];
    int start = 0;
    int factor = ceil((float)numberOfElements / PROCESS_MAX);

    int process1Array[qSizeArray[0]];
    int process2Array[qSizeArray[1]];
    int process3Array[qSizeArray[2]];
    int process4Array[qSizeArray[3]];

    int mid = ceil((float)numberOfElements / 2);
    int firstHalf[mid];
    int secondHalf[numberOfElements - mid];

    clock_t initial;
    initial = clock();

    for (int i = 0; i < PROCESS_MAX; i++) {
        pipe(qfd[i]);
    }

    process[0] = fork();

    if (process[0] == 0) {
        int tempArray[qSizeArray[start]];
        quickSort(quickPDataArray, start * factor, ((start + 1) * factor) - 1);
        for (int i = 0, j = start * factor; i < qSizeArray[start]; i++, j++) {
            tempArray[i] = quickPDataArray[j];
        }
        write(qfd[start][1], &tempArray, qSizeArray[start] * sizeof(int));
        exit(0);
    } else if (process[0] > 0) {
        process[1] = fork();
        start++;
        if (process[1] == 0) {
            int tempArray[qSizeArray[start]];
            quickSort(quickPDataArray, start * factor, ((start + 1) * factor) - 1);
            for (int i = 0, j = start * factor; i < qSizeArray[start]; i++, j++) {
                tempArray[i] = quickPDataArray[j];
            }
            write(qfd[start][1], &tempArray, qSizeArray[start] * sizeof(int));
            exit(0);
        } else if (process[1] > 0) {
            process[2] = fork();
            start++;
            if (process[2] == 0) {
                int tempArray[qSizeArray[start]];
                quickSort(quickPDataArray, start * factor, ((start + 1) * factor) - 1);
                for (int i = 0, j = start * factor; i < qSizeArray[start]; i++, j++) {
                    tempArray[i] = quickPDataArray[j];
                }
                write(qfd[start][1], &tempArray, qSizeArray[start] * sizeof(int));
                exit(0);
            } else if (process[2] > 0) {
                process[3] = fork();
                start++;
                if (process[3] == 0) {
                    int tempArray[qSizeArray[start]];
                    quickSort(quickPDataArray, start * factor, ((start + 1) * factor) - 1);
                    for (int i = 0, j = start * factor; i < qSizeArray[start]; i++, j++) {
                        tempArray[i] = quickPDataArray[j];
                    }
                    write(qfd[start][1], &tempArray, qSizeArray[start] * sizeof(int));
                    exit(0);
                }
            }
        }
        wait(NULL);
        read(qfd[0][0], &process1Array, qSizeArray[0] * sizeof(int));
        read(qfd[1][0], &process2Array, qSizeArray[1] * sizeof(int));
        read(qfd[2][0], &process3Array, qSizeArray[2] * sizeof(int));
        read(qfd[3][0], &process4Array, qSizeArray[3] * sizeof(int));

        process[0] = fork();

        if (process[0] == 0) {
            quickMerge(firstHalf, process1Array, process2Array, qSizeArray[0], qSizeArray[1]);
            write(qfd[0][1], &firstHalf, mid * sizeof(int));
            exit(0);

        } else if (process[0] > 0) {
            process[1] = fork();

            if (process[1] == 0) {
                quickMerge(secondHalf, process3Array, process4Array, qSizeArray[2], qSizeArray[3]);
                write(qfd[1][1], &secondHalf, (numberOfElements - mid) * sizeof(int));
                exit(0);
            }

            wait(NULL);

            // reading from pipes
            read(qfd[0][0], &firstHalf, mid * sizeof(int));
            read(qfd[1][0], &secondHalf, (numberOfElements - mid) * sizeof(int));

            // merging firstHalf and secondHalf into original mergePDataArray
            quickMerge(quickPDataArray, firstHalf, secondHalf, mid, numberOfElements - mid);

            // printf("\nElements after sort:\n");
            // for (int i = 0; i < numberOfElements; i++) {
            //     printf("%d ", quickPDataArray[i]);
            // }
            // printf("\n");
        }
    }

    initial = clock() - initial;
    printf("\nTime taken for soriting using process: %f seconds\n", (float)initial / CLOCKS_PER_SEC);
}
#endif