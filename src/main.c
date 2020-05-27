#include <stdio.h>

#include "../includes/merge.h"
#include "../includes/quick.h"
#include "../includes/selection.h"

int main(int argc, char const *argv[]) {
    makeArray();
    setter();
    selectionSortProcess();
    selectionSortThread();
    mergeSortProcess();
    mergeSortThread();
    quickSortThread();
    quickSortProcess();
    cleanArray();
    return 0;
}
