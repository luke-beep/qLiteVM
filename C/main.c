#include <stdio.h>
#include <signal.h>
#include "io_control.h"
#include "vm_memory.h"
#include "vm_utilities.h"
#include "vm_core.h"
#include "vm_image.h"
#include "vm_instructions.h"
#include "vm_globals.h"
#include "main.h"
#include <Windows.h>

uint16_t reg[R_COUNT]; // 8 registers
uint16_t memory[MEMORY_MAX]; // 65536 memory locations

int running = 1;

int main(int argc, const char* argv[])
{
    const char* USAGE = "Usage: qLiteVM [image-file1] [image-file2] ...\n";
    const char* VERSION = "qLiteVM v1.0.0\n";

    if (argc < 2)
    {
        printf("%s", USAGE);
        exit(2);
    }
    for (int j = 1; j < argc; ++j)
    {
        if (!ReadImage(argv[j]))
        {
            printf("failed to load image: %s\n", argv[j]);
            exit(1);
        }
    }

    // Setup
    printf("%s", VERSION);

    signal(SIGINT, HandleInterrupt);
    DisableInputBuffering();

    reg[R_COND] = FL_ZRO;
    reg[R_PC] = PC_START;

    // Main Loop
    while (running)
    {
        uint16_t instr = MemoryRead(reg[R_PC]++);
        uint16_t op = instr >> 12;

        switch (op)
        {
            case OP_ADD:
            {
                HandleADD(instr);
            }
                break;
            case OP_AND:
            {
                HandleAND(instr);
            }
                break;
            case OP_NOT:
            {
                HandleNOT(instr);
            }
                break;
            case OP_BR:
            {
                HandleBR(instr);
            }
                break;
            case OP_JMP:
            {
                HandleJMP(instr);
            }
                break;
            case OP_JSR:
            {
                HandleJSR(instr);
            }
                break;
            case OP_LD:
            {
                HandleLD(instr);
            }
                break;
            case OP_LDI:
            {
                HandleLDI(instr);
            }
                break;
            case OP_LDR:
            {
                HandleLDR(instr);
            }
                break;
            case OP_LEA:
            {
                HandleLEA(instr);
            }
                break;
            case OP_ST:
            {
                HandleST(instr);
            }
                break;
            case OP_STI:
            {
                HandleSTI(instr);
            }
                break;
            case OP_STR:
            {
                HandleSTR(instr);
            }
                break;
            case OP_TRAP: {
                HandleTRAP(instr);
            }
                break;
            case OP_RES:
            case OP_RTI:
            default:
                abort();
        }
    }
    RestoreInputBuffering();
}

void HandleADD(uint16_t instr){
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t r1 = (instr >> 6) & 0x7;
    uint16_t imm_flag = (instr >> 5) & 0x1;

    if (imm_flag)
    {
        uint16_t imm5 = SignExtend(instr & 0x1F, 5);
        reg[r0] = reg[r1] + imm5;
    }
    else
    {
        uint16_t r2 = instr & 0x7;
        reg[r0] = reg[r1] + reg[r2];
    }

    UpdateFlags(r0);
}

void HandleAND(uint16_t instr){
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t r1 = (instr >> 6) & 0x7;
    uint16_t imm_flag = (instr >> 5) & 0x1;

    if (imm_flag)
    {
        uint16_t imm5 = SignExtend(instr & 0x1F, 5);
        reg[r0] = reg[r1] & imm5;
    }
    else
    {
        uint16_t r2 = instr & 0x7;
        reg[r0] = reg[r1] & reg[r2];
    }
    UpdateFlags(r0);
}

void HandleNOT(uint16_t instr){
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t r1 = (instr >> 6) & 0x7;

    reg[r0] = ~reg[r1];
    UpdateFlags(r0);
}

void HandleBR(uint16_t instr){
    uint16_t pc_offset = SignExtend(instr & 0x1FF, 9);
    uint16_t cond_flag = (instr >> 9) & 0x7;
    if (cond_flag & reg[R_COND])
    {
        reg[R_PC] += pc_offset;
    }
}

void HandleJMP(uint16_t instr){
    uint16_t r1 = (instr >> 6) & 0x7;
    reg[R_PC] = reg[r1];
}

void HandleJSR(uint16_t instr){
    uint16_t long_flag = (instr >> 11) & 1;
    reg[R_R7] = reg[R_PC];
    if (long_flag)
    {
        uint16_t long_pc_offset = SignExtend(instr & 0x7FF, 11);
        reg[R_PC] += long_pc_offset;
    }
    else
    {
        uint16_t r1 = (instr >> 6) & 0x7;
        reg[R_PC] = reg[r1];
    }
}

void HandleLD(uint16_t instr){
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t pc_offset = SignExtend(instr & 0x1FF, 9);
    reg[r0] = MemoryRead(reg[R_PC] + pc_offset);
    UpdateFlags(r0);
}

void HandleLDI(uint16_t instr){
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t pc_offset = SignExtend(instr & 0x1FF, 9);
    reg[r0] = MemoryRead(MemoryRead(reg[R_PC] + pc_offset));
    UpdateFlags(r0);
}

void HandleLDR(uint16_t instr){
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t r1 = (instr >> 6) & 0x7;
    uint16_t offset = SignExtend(instr & 0x3F, 6);
    reg[r0] = MemoryRead(reg[r1] + offset);
    UpdateFlags(r0);
}

void HandleLEA(uint16_t instr){
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t pc_offset = SignExtend(instr & 0x1FF, 9);
    reg[r0] = reg[R_PC] + pc_offset;
    UpdateFlags(r0);
}

void HandleST(uint16_t instr){
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t pc_offset = SignExtend(instr & 0x1FF, 9);
    MemoryWrite(reg[R_PC] + pc_offset, reg[r0]);
}

void HandleSTI(uint16_t instr){
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t pc_offset = SignExtend(instr & 0x1FF, 9);
    MemoryWrite(MemoryRead(reg[R_PC] + pc_offset), reg[r0]);
}

void HandleSTR(uint16_t instr){
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t r1 = (instr >> 6) & 0x7;
    uint16_t offset = SignExtend(instr & 0x3F, 6);
    MemoryWrite(reg[r1] + offset, reg[r0]);
}

void HandleTRAP(uint16_t instr){
    reg[R_R7] = reg[R_PC];

    switch (instr & 0xFF) {
        case TRAP_GETC:
            reg[R_R0] = (uint16_t) getchar();
            UpdateFlags(R_R0);
            break;
        case TRAP_OUT:
            putc((char) reg[R_R0], stdout);
            fflush(stdout);
            break;
        case TRAP_PUTS: {
            uint16_t *c = memory + reg[R_R0];
            while (*c) {
                putc((char) *c, stdout);
                ++c;
            }
            fflush(stdout);
        }
            break;
        case TRAP_IN: {
            printf("Enter a character: ");
            int c = getchar();
            putc(c, stdout);
            fflush(stdout);
            reg[R_R0] = (uint16_t) c;
            UpdateFlags(R_R0);
        }
            break;
        case TRAP_PUTSP: {
            uint16_t *c = memory + reg[R_R0];
            while (*c) {
                int char1 = (*c) & 0xFF;
                putc(char1, stdout);
                int char2 = (*c) >> 8;
                if (char2) putc(char2, stdout);
                ++c;
            }
            fflush(stdout);
        }
            break;
        case TRAP_HALT:{
            puts("HALT");
            fflush(stdout);
            Exit();
        }
            break;
    }
}

void Exit(){
    running = 0;
}