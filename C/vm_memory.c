#include <stdio.h>
#include "vm_memory.h"
#include "io_control.h"
#include "vm_globals.h"

void HandleKeyboardInput() {
    if (CheckKey()) {
        memory[MR_KBSR] = KEYBOARD_FLAG;
        memory[MR_KBDR] = getchar();
    } else {
        memory[MR_KBSR] = 0;
    }
}

/**
 * Writes a value to a specified memory address in the LC3 virtual machine.
 *
 * This function writes a 16-bit value to a specific address in the LC3 VM's memory.
 * It checks if the address is within the bounds of the memory array. If the address
 * is out of bounds, it prints an error message to standard error.
 *
 */
void MemoryWrite(uint16_t address, uint16_t val) {
    if (MEMORY_MAX > address) {
        memory[address] = val;
    } else {
        fprintf(stderr, "Memory write error: Address out of bounds\n");
    }
}

/**
 * Reads a value from a specified memory address in the LC3 virtual machine.
 *
 * This function reads a 16-bit value from a specific address in the LC3 VM's memory.
 * If the address is the keyboard status register (MR_KBSR), it triggers a function
 * to handle keyboard input. If the memory address is out of bounds, it prints an
 * error message to standard error and returns 0.
 *
 */
uint16_t MemoryRead(uint16_t address) {
    if (MEMORY_MAX > address) {
        if (address == MR_KBSR) {
            HandleKeyboardInput();
        }
        return memory[address];
    } else {
        fprintf(stderr, "Memory read error: Address out of bounds\n");
        return 0;
    }
}