#include <stdio.h>

#include "../includes/merge.h"
#include "../includes/selection.h"

int main(int argc, char const *argv[]) {
    makeArray();
    // selectionSortProcess();
    // selectionSortThread();
    mergeSortThread();
    cleanArray();
    return 0;
}
