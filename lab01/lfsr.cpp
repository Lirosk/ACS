#include <iostream>

#define r *reg

void lfsr_calculate(uint16_t *reg) {
    /* YOUR CODE HERE */

    uint16_t temp = (r ^ (r >> 2) ^ (r >> 3) ^ (r >> 5)) << 15;
    r >>= 1;
    r |= temp;
}