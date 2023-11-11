#ifndef VM_MEMORY_H
#define VM_MEMORY_H

#include <stdint.h>

// Memory-mapped I/O addresses for the LC3 VM
enum {
    MR_KBSR = 0xFE00, // Keyboard Status
    MR_KBDR = 0xFE02  // Keyboard Data
};

/**
 * Writes a value to a specified memory address in the LC3 virtual machine.
 *
 * @param address The memory address where the value needs to be written.
 * @param val The value to write to the memory address.
 *
 */
void MemoryWrite(uint16_t address, uint16_t val);

/**
 * Reads a value from a specified memory address in the LC3 virtual machine.
 *
 * @param address The memory address from which the value needs to be read.
 * @return The value read from the specified memory address.
 *
 */
uint16_t MemoryRead(uint16_t address);

#endif