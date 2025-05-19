#include "math.h"
#include <assert.h>

int main() {
    
    complex_t A = {1, 3};
    complex_t B = {3, 4};
    complex_t A_PLUS_B = {4, 8};
    
    assert(complex_equal(complex_add(A, B), A_PLUS_B) == TRUE);
}