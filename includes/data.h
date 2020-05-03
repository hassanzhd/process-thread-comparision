#ifndef DATA_H
#define DATA_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *array;
int numberOfElements = 0;

void makeArray() {
    srand(time(0));

    printf("Enter number of elements: ");
    scanf("%d", &numberOfElements);
    array = (int *)calloc(numberOfElements, sizeof(int));
    for (int i = 0; i < numberOfElements; i++) {
        array[i] = rand() % 100000;
        printf("%d\n", array[i]);
    }
}

void cleanArray() {
    free(array);
}

#endif