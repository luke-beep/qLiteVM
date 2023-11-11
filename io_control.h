#ifndef IO_CONTROL_H
#define IO_CONTROL_H
#define WAIT_TIME 1000

#include <stdint.h>

/**
 * Disables line input and echo input modes in the console.
 *
 */
void DisableInputBuffering();

/**
 * Restores the original input buffering settings of the console.
 *
 */
void RestoreInputBuffering();

/**
 * Checks if a key has been pressed in the console.
 *
 * @return A nonzero value if a key is pressed; otherwise, 0.
 *
 */
uint16_t CheckKey();

#endif