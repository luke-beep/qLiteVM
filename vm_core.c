#include "vm_core.h"
#include "io_control.h"
#include "vm_globals.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Handles an interrupt signal.
 *
 * This function is designed to be called when an interrupt signal is received by the program.
 * It performs necessary cleanup tasks such as restoring the input buffering state. It then
 * prints a newline character for readability and exits the program with an error code (-2).
 * This function is typically used as a signal handler for abrupt termination signals like SIGINT.
 */
void HandleInterrupt(int signal)
{
    RestoreInputBuffering();
    printf("\n");
    exit(-2);
}

/**
 * Updates the condition flags in the LC3 virtual machine.
 *
 * This function updates the condition flags in the R_COND register based on the value of
 * the specified register (denoted by 'r'). The flags set are: FL_ZRO if the register value
 * is zero, FL_NEG if the register value is negative (sign bit is set), and FL_POS otherwise
 * (i.e., the value is positive).
 */
void UpdateFlags(uint16_t r)
{
    if (reg[r] == 0)
    {
        reg[R_COND] = FL_ZRO;
    }
    else if (reg[r] >> 15)
    {
        reg[R_COND] = FL_NEG;
    }
    else
    {
        reg[R_COND] = FL_POS;
    }
}