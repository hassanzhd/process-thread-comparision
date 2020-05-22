#ifndef DATA_H
#define DATA_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define THREAD_MAX 4  // maximum number of threads depending upon cores
#define PROCESS_MAX 4

int numberOfElements = 0;  // total number of array elements
int *selectionTDataArray;  // Array for selection sort thread implementation
int *selectionPDataArray;  // Array for selection sort process implementation
int *mergeTDataArray;      // Array for merge sort thread implementation
int *mergePDataArray;      // Array for merge sort process implementation
int *quickTDataArray;      // Array for quick sort thread implementation

void makeArray() {
    srand(time(0));

    printf("\nEnter number of elements: ");
    scanf("%d", &numberOfElements);
    selectionTDataArray = (int *)calloc(numberOfElements, sizeof(int));
    selectionPDataArray = (int *)calloc(numberOfElements, sizeof(int));
    mergeTDataArray = (int *)calloc(numberOfElements, sizeof(int));
    mergePDataArray = (int *)calloc(numberOfElements, sizeof(int));
    quickTDataArray = (int *)calloc(numberOfElements, sizeof(int));

    printf("\nOriginal Array:\n");
    for (int i = 0, j = numberOfElements; i < numberOfElements; i++, j--) {
        selectionTDataArray[i] = selectionPDataArray[i] = mergeTDataArray[i] = mergePDataArray[i] = quickTDataArray[i] = rand() % 100000;
        printf("%d ", selectionTDataArray[i]);
    }
    printf("\n");
}

void cleanArray() {
    // // free(selectionTDataArray);
    // // free(selectionPDataArray);
    // // free(mergeTDataArray);
    // // free(mergePDataArray);
    free(quickTDataArray);
}

#endif