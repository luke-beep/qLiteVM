#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

enum
{
    PC_START = 0x3000
};

extern int running;

void HandleADD(uint16_t instr);
void HandleAND(uint16_t instr);
void HandleNOT(uint16_t instr);
void HandleBR(uint16_t instr);
void HandleJMP(uint16_t instr);
void HandleJSR(uint16_t instr);
void HandleLD(uint16_t instr);
void HandleLDI(uint16_t instr);
void HandleLDR(uint16_t instr);
void HandleLEA(uint16_t instr);
void HandleST(uint16_t instr);
void HandleSTI(uint16_t instr);
void HandleSTR(uint16_t instr);
void HandleTRAP(uint16_t instr);

void Exit();

#endif