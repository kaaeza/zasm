#ifndef EMULATOR_H
#define EMULATOR_H

#include "utils.h"
#include "z80.h"
#include "lexer.h"

/// @brief Executes an array of instructions
/// @param instructions Array of instructions to execute
void execFile(Instruction instructions[]);

/// @brief Returns a pointer to a register of the cpu
/// @param reg Character reèresenting the register ('A', 'B'...)
/// @return Pointer to the corresponding register
uint8_t *getReg8Bit(const char reg);

/// @brief Returns a pointer to the value represented by the instruction argument.
/// @param arg The argument string (register, immediate hex value, or memory address).
/// @return Pointer to the 8-bit value corresponding to the argument, or NULL if invalid.
uint8_t *handleArg(char *arg);
#endif