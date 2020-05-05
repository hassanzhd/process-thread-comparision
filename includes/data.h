#ifndef DATA_H
#define DATA_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *selectionArray;
int numberOfElements = 0;

void makeArray() {
    srand(time(0));

    printf("\nEnter number of elements: ");
    scanf("%d", &numberOfElements);
    selectionArray = (int *)calloc(numberOfElements, sizeof(int));
    for (int i = 0; i < numberOfElements; i++) {
        selectionArray[i] = rand() % 1000000;
        printf("%d ", selectionArray[i]);
    }
    printf("\n");
}

void cleanArray() {
    free(selectionArray);
}

#endif