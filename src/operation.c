#include "operation.h"
#include <stdlib.h>
#include <stdio.h>

void free_operation(operation_t* operation) {
    free(operation->qubit_indices);
    free(operation);
}