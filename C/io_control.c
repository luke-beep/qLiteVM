#include "io_control.h"
#include <Windows.h>
#include <conio.h>

HANDLE hStdin = INVALID_HANDLE_VALUE;
DWORD fdwMode, fdwOldMode;

/**
 * Disables line input and echo input modes in the console.
 *
 * This function retrieves the current input mode of the console, disables line input and echo input
 * modes, and sets this new mode. It's typically used to allow the console to handle input more
 * directly, which is suitable for scenarios where immediate input response is needed. It also
 * flushes the console input buffer to ensure a clean state.
 */
void DisableInputBuffering()
{
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hStdin, &fdwOldMode);
    fdwMode = fdwOldMode
              ^ ENABLE_ECHO_INPUT
              ^ ENABLE_LINE_INPUT;

    SetConsoleMode(hStdin, fdwMode);
    FlushConsoleInputBuffer(hStdin);
}

/**
 * Restores the original input buffering settings of the console.
 *
 * This function resets the console mode to its original settings, effectively re-enabling line
 * input and echo input modes if they were previously disabled. This is typically called before
 * the program exits or when the program no longer requires direct input handling.
 */
void RestoreInputBuffering()
{
    SetConsoleMode(hStdin, fdwOldMode);
}

/**
 * Checks if a key has been pressed in the console.
 *
 * This function waits for an event on the standard input (with a specified timeout) and then checks
 * if a key has been pressed (non-blocking). It's used to poll the console for keyboard input without
 * pausing execution of the program. The WAIT_TIME constant defines the wait duration.
 */
uint16_t CheckKey()
{
    return WaitForSingleObject(hStdin, WAIT_TIME) == WAIT_OBJECT_0 && _kbhit();
}