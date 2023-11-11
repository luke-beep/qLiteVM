#ifndef VM_REGISTERS_H
#define VM_REGISTERS_H

/**
 * Enum for LC3 Virtual Machine Registers.
 *
 * R_R0 to R_R7 are general-purpose registers.
 * R_PC is the Program Counter, pointing to the current instruction.
 * R_COND is the Condition Register, used for setting condition flags.
 * R_COUNT is a utility value to keep track of the number of registers.
 */
enum {
    R_R0 = 0,  // General-purpose register 0
    R_R1,      // General-purpose register 1
    R_R2,      // General-purpose register 2
    R_R3,      // General-purpose register 3
    R_R4,      // General-purpose register 4
    R_R5,      // General-purpose register 5
    R_R6,      // General-purpose register 6
    R_R7,      // General-purpose register 7
    R_PC,      // Program Counter
    R_COND,    // Condition Flags Register
    R_COUNT    // Total number of registers
};

/**
 * Enum for Condition Flags in LC3 Virtual Machine.
 *
 * FL_POS, FL_ZRO, and FL_NEG are flags set in the R_COND register
 * to indicate the result of an operation (e.g., if the result is
 * positive, zero, or negative).
 *
 */
enum {
    FL_POS = 1 << 0,  // Positive flag
    FL_ZRO = 1 << 1,  // Zero flag
    FL_NEG = 1 << 2,  // Negative flag
};


#endif