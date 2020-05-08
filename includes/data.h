#ifndef DATA_H
#define DATA_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int numberOfElements = 0;
int *selectionTDataArray;  // Array for Thread implementation

int *selectionPDataArray;  // Array for process implementation

void makeArray() {
    srand(time(0));

    printf("\nEnter number of elements: ");
    scanf("%d", &numberOfElements);
    selectionTDataArray = (int *)calloc(numberOfElements, sizeof(int));
    selectionPDataArray = (int *)calloc(numberOfElements, sizeof(int));
    printf("\nOriginal Array:\n");
    for (int i = 0; i < numberOfElements; i++) {
        selectionTDataArray[i] = selectionPDataArray[i] = rand() % 100000;
        printf("%d ", selectionTDataArray[i]);
    }
    printf("\n");
}

void cleanArray() {
    free(selectionTDataArray);
    free(selectionPDataArray);
}

#endif