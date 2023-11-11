#ifndef VM_GLOBALS_H
#define VM_GLOBALS_H
#define MEMORY_MAX    (1 << 16) // 2^16 = 65536
#define KEYBOARD_FLAG (1 << 15) // 2^15 = 32768

#include <stdint.h>
#include "vm_memory.h"
#include "vm_registers.h"
#include "vm_utilities.h"

extern uint16_t memory[MEMORY_MAX];
extern uint16_t reg[R_COUNT];

#endif