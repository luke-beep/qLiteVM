#include "vm_utilities.h"

uint16_t SignExtend(uint16_t x, int bit_count)
{
    if ((x >> (bit_count - 1)) & 1) {
        x |= (0xFFFF << bit_count);
    }
    return x;
}

uint16_t Swap16(uint16_t x)
{
    return (x << 8) | (x >> 8);
}