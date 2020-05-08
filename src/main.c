#include <stdio.h>

#include "../includes/selection.h"

int main(int argc, char const *argv[]) {
    makeArray();
    selectionSortProcess();
    selectionSortThread();
    cleanArray();
    return 0;
}
