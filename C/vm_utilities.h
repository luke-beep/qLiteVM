#ifndef VM_UTILITIES_H
#define VM_UTILITIES_H

#include <stdint.h>

/**
 * Sign extends a smaller bit number to a 16-bit number.
 *
 * @param x The number to be sign extended.
 * @param bit_count The original size in bits of `x`.
 * @return The sign-extended version of `x`.
 *
 */
uint16_t SignExtend(uint16_t x, int bit_count);

/**
 * Swaps the bytes of a 16-bit number.
 *
 * @param x The number whose bytes are to be swapped.
 * @return The byte-swapped version of `x`.
 *
 */
uint16_t Swap16(uint16_t x);

#endif
