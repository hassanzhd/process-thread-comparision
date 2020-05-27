#ifndef DATA_H
#define DATA_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int numberOfElements = 0;  // total number of array elements
int *selectionTDataArray;  // Array for selection sort thread implementation
int *selectionPDataArray;  // Array for selection sort process implementation
int *mergeTDataArray;      // Array for merge sort thread implementation
int *mergePDataArray;      // Array for merge sort process implementation
int *quickTDataArray;      // Array for quick sort thread implementation
int *quickPDataArray;      // Array for quick sort process implementation

#include "merge.h"
#include "quick.h"

void makeArray() {
    int range;

    srand(time(0));

    printf("\nEnter number of elements: ");
    scanf("%d", &numberOfElements);
    printf("Enter range: ");
    scanf("%d", &range);
    selectionTDataArray = (int *)calloc(numberOfElements, sizeof(int));
    selectionPDataArray = (int *)calloc(numberOfElements, sizeof(int));
    mergeTDataArray = (int *)calloc(numberOfElements, sizeof(int));
    mergePDataArray = (int *)calloc(numberOfElements, sizeof(int));
    quickTDataArray = (int *)calloc(numberOfElements, sizeof(int));
    quickPDataArray = (int *)calloc(numberOfElements, sizeof(int));

    printf("\nOriginal Array:\n");
    for (int i = 0, j = numberOfElements; i < numberOfElements; i++, j--) {
        selectionTDataArray[i] = selectionPDataArray[i] = mergeTDataArray[i] = mergePDataArray[i] = quickTDataArray[i] = quickPDataArray[i] = rand() % range;
        printf("%d ", selectionTDataArray[i]);
    }
    printf("\n");
}

void setter() {
    int num = numberOfElements, i = 0, j = 4;
    while (num > 0) {
        int value = ceil((float)num / j);
        mSizeArray[i] = qSizeArray[i] = value;
        num -= value;
        i++;
        j--;
    }  // calculating sizes of different partitions as array is divided into 4 parts

    for (int i = 0; i < THREAD_MAX; i++) {
        int value = 0;
        for (int j = 0; j < i; j++) {
            value += mSizeArray[j];
        }
        increment[i] = value;
    } /*  calculating increment values that will be needed to merge 4 paritions into 2 
    i.e firstHalf and secondHalf */
}

void cleanArray() {
    free(selectionTDataArray);
    free(selectionPDataArray);
    free(mergeTDataArray);
    free(mergePDataArray);
    free(quickTDataArray);
    free(quickPDataArray);
}

#endif