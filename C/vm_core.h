#ifndef VM_CORE_H
#define VM_CORE_H

#include <stdint.h>

/**
 * Handles an interrupt signal.
 *
 * @param signal The signal number that triggered the interrupt.
 *
 */
void HandleInterrupt(int signal);

/**
 * Updates the condition flags in the LC3 virtual machine.
 *
 * @param r The index of the register whose value is used to set the condition flags.
 *
 */
void UpdateFlags(uint16_t r);

#endif