/// @file z80.h
/// @brief Header file defining the Z80 CPU registers, flags, and memory for the Z80 emulator.

#ifndef Z80_H
#define Z80_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/// @brief Struct which contains the CPU flags (F register)
typedef struct {
    bool SF;
    bool ZF;
    bool YF;
    bool HF;
    bool XF;
    bool PF;
    bool NF; // adds = 0, subs = 1
    bool CF;
} flagsRegister;

/// @brief Struct which contains the CPU registers
typedef struct {
    // 8 bit
    uint8_t A; ///< Accumulator
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    flagsRegister F; ///< Flags register
    uint8_t H;
    uint8_t L;

    // 16 bit
    uint16_t PC; // program counter
    uint16_t SP; // stack pointer
    uint16_t IX; // index register X
    uint16_t IY; // index register Y
} z80;

/// @brief Global CPU instance
extern z80 cpu;

/// @brief 64KB memory
extern uint8_t memory[0x10000];

/// @brief Debug program flag
extern bool debugActive;
#endif
