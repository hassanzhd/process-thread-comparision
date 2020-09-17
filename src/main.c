#include <stdio.h>

#include "../includes/merge.h"
#include "../includes/quick.h"
#include "../includes/selection.h"

void selectionAnalysis() {
    printf("\n\t\t\t\t\t\t\t==============\n");
    printf("\t\t\t\t\t\t\tSELECTION SORT\n");
    printf("\t\t\t\t\t\t\t==============\n");
    selectionSortThread();
    selectionSortProcess();
}

int mergeAnalysis() {
    printf("\n\t\t\t\t\t\t\t==========\n");
    printf("\t\t\t\t\t\t\tMERGE SORT\n");
    printf("\t\t\t\t\t\t\t==========\n");
    mergeSortThread();
    mergeSortProcess();
}

int quickAnalysis() {
    printf("\n\t\t\t\t\t\t\t==========\n");
    printf("\t\t\t\t\t\t\tQUICK SORT\n");
    printf("\t\t\t\t\t\t\t==========\n");
    quickSortThread();
    quickSortProcess();
}

int main(int argc, char const *argv[]) {
    makeArray();
    setter();
    mergeAnalysis();
    quickAnalysis();
    selectionAnalysis();
    cleanArray();
    return 0;
}
