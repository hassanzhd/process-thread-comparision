#ifndef MERGE_H
#define MERGE_H
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

void merge(int __original[], int __left[], int __right[], int __llength, int __rlength) {
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
void mergeSort(int __array[], int n) {
    if (n < 2) {
        return;
    } else {
        int mid = ceil((float)n / 2);
        int leftArr[mid];
        int rightArr[n - mid];

        for (int i = 0; i < mid; i++) {
            leftArr[i] = __array[i];
        }

        for (int i = mid; i < n; i++) {
            rightArr[i - mid] = __array[i];
        }
        mergeSort(leftArr, mid);
        mergeSort(rightArr, n - mid);
        merge(__array, leftArr, rightArr, mid, n - mid);
    }
}

int *divideArray;                  // for mergeSort function
int *mergeArray;                   // for merge function
int sizeArray[THREAD_MAX] = {-1};  // size of different partitions (i.e 4 different divisions)
int increment[THREAD_MAX] = {0};   // arrays that will be used to increment merge array for merge function

void msSetter() {
    int num = numberOfElements, i = 0, j = 4;
    while (num > 0) {
        int value = ceil((float)num / j);
        sizeArray[i] = value;
        num -= value;
        i++;
        j--;
    }  // calculating sizes of different partitions as array is divided into 4 parts

    for (int i = 0; i < THREAD_MAX; i++) {
        int value = 0;
        for (int j = 0; j < i; j++) {
            value += sizeArray[j];
        }
        increment[i] = value;
    } /*  calculating increment values that will be needed to merge 4 paritions into 2 
    i.e firstHalf and secondHalf */
}

int divideThreadNumber = 0;

void *msThread(void *__args) {
    int factor = divideThreadNumber++;
    mergeSort(divideArray, sizeArray[factor]);
    divideArray += sizeArray[factor];
}

void *mergingThread(void *__args) {
    int first = divideThreadNumber++;
    int second = divideThreadNumber++;
    merge((int *)__args, mergeArray + (increment[first]), mergeArray + (increment[second]), sizeArray[first], sizeArray[second]);
}

void mergeSortThread() {
    printf("\n=====================\n");
    printf("SORTING USING THREAD\n");
    printf("=====================\n");

    divideArray = mergeTDataArray;
    mergeArray = mergeTDataArray;
    pthread_t threads[THREAD_MAX];

    clock_t start;
    start = clock();

    int mid = ceil((float)numberOfElements / 2);
    int firstHalf[mid];
    int secondHalf[numberOfElements - mid];

    for (int i = 0; i < THREAD_MAX; i++) {
        pthread_create(&threads[i], NULL, msThread, (void *)NULL);
    }

    for (int i = 0; i < THREAD_MAX; i++) {
        pthread_join(threads[i], NULL);
    }

    /* The four different parts will be combined into 2 (firstHalf and secondHalf) and then 
    completely merged into a single array */

    divideThreadNumber = 0;
    pthread_create(&threads[0], NULL, mergingThread, &firstHalf);
    pthread_create(&threads[1], NULL, mergingThread, &secondHalf);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    merge(mergeTDataArray, firstHalf, secondHalf, mid, numberOfElements - mid);

    printf("\nElements after sorting:\n");
    for (int i = 0; i < numberOfElements; i++) {
        printf("%d ", mergeTDataArray[i]);
    }
    printf("\n");

    start = clock() - start;
    printf("\nTime taken for sorting using threads: %f seconds\n", (float)start / CLOCKS_PER_SEC);
}

int divideProcessNumber = 0;
int fd[PROCESS_MAX][2];

void msProcess() {
    int factor = divideProcessNumber, tempArray[sizeArray[factor]];
    mergeSort(divideArray, sizeArray[factor]);
    for (int i = 0; i < sizeArray[factor]; i++) {
        tempArray[i] = divideArray[i];
    }
    write(fd[factor][1], &tempArray, sizeArray[factor] * sizeof(int));
}

void mergeSortProcess() {
    printf("\n=====================\n");
    printf("SORTING USING PROCESS\n");
    printf("=====================\n");

    pid_t process[PROCESS_MAX];
    divideArray = mergePDataArray;
    mergeArray = mergePDataArray;

    int process1Array[sizeArray[0]];
    int process2Array[sizeArray[1]];
    int process3Array[sizeArray[2]];
    int process4Array[sizeArray[3]];

    int mid = ceil((float)numberOfElements / 2);
    int firstHalf[mid];
    int secondHalf[numberOfElements - mid];

    clock_t start;
    start = clock();

    for (int i = 0; i < PROCESS_MAX; i++) {
        pipe(fd[i]);
    }

    process[0] = fork();
    if (process[0] == 0) {
        msProcess();
        exit(0);
    } else if (process[0] > 0) {
        process[1] = fork();
        divideArray += sizeArray[divideProcessNumber];
        divideProcessNumber++;

        if (process[1] == 0) {
            msProcess();
            exit(0);
        } else if (process[1] > 0) {
            process[2] = fork();
            divideArray += sizeArray[divideProcessNumber];
            divideProcessNumber++;

            if (process[2] == 0) {
                msProcess();
                exit(0);
            } else if (process[2] > 0) {
                process[3] = fork();
                divideArray += sizeArray[divideProcessNumber];
                divideProcessNumber++;

                if (process[3] == 0) {
                    msProcess();
                    exit(0);
                }
            }
        }

        wait(NULL);
        read(fd[0][0], &process1Array, sizeArray[0] * sizeof(int));
        read(fd[1][0], &process2Array, sizeArray[1] * sizeof(int));
        read(fd[2][0], &process3Array, sizeArray[2] * sizeof(int));
        read(fd[3][0], &process4Array, sizeArray[3] * sizeof(int));

        process[0] = fork();

        if (process[0] == 0) {
            merge(firstHalf, process1Array, process2Array, sizeArray[0], sizeArray[1]);
            write(fd[0][1], &firstHalf, mid * sizeof(int));
            exit(0);

        } else if (process[0] > 0) {
            process[1] = fork();

            if (process[1] == 0) {
                merge(secondHalf, process3Array, process4Array, sizeArray[2], sizeArray[3]);
                write(fd[1][1], &secondHalf, (numberOfElements - mid) * sizeof(int));
                exit(0);
            }

            wait(NULL);
            read(fd[0][0], &firstHalf, mid * sizeof(int));
            read(fd[1][0], &secondHalf, (numberOfElements - mid) * sizeof(int));
            merge(mergePDataArray, firstHalf, secondHalf, mid, numberOfElements - mid);

            printf("\nElements after sorting:\n");
            for (int i = 0; i < numberOfElements; i++) {
                printf("%d ", mergePDataArray[i]);
            }
            printf("\n");
        }
    }

    start = clock() - start;
    printf("\nTime taken for sorting using process: %f seconds\n", (float)start / CLOCKS_PER_SEC);
}

#endif