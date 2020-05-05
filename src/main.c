#include <stdio.h>

#include "../includes/data.h"
#include "../includes/selection.h"

int main(int argc, char const *argv[]) {
    makeArray();
    selectionSortThread();
    cleanArray();
    return 0;
}
