#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdint.h>

/// @brief Types of instruction mnemonics
typedef enum {
    IST_LD,
    // 8-BIT Arithmetic and logical group
    IST_ADD,
    IST_SUB,
    IST_AND,
    IST_OR,
    IST_XOR,
    IST_CP,
    IST_INC,
    IST_DEC,
    IST_ADC,
    IST_SBC,
    // Jump Group
    IST_JP,
    IST_JR,
    IST_DJNZ,
    // General Purpose
    IST_HALT,
    IST_NOP,
    IST_CCF,
    IST_SCF,
    IST_NEG,
    IST_ERR
} InstructionType;

/// @brief Structure representing an instruction
typedef struct {
    uint16_t IC;            ///< Instruction counter
    InstructionType instType; ///< Type of instruction
    char arg1[10];          ///< First argument (register, value, etc.)
    char arg2[10];          ///< Second argument (register, value, etc.)
} Instruction;

/// @brief Converts a mnemonic string to its corresponding InstructionType
/// @param instType Null-terminated string representing the instruction mnemonic
/// @return Corresponding InstructionType enum value
InstructionType handleInstType(char *instType);

/// @brief Parses a line of code into an Instruction struct
/// @param line Null-terminated string representing the line of code
/// @param IC Instruction counter / address for this instruction
/// @return Instruction struct representing the parsed line
Instruction parseLine(char line[], uint16_t IC);

/// @brief Loads instructions from a file into an array
/// @param filename Path to the file containing instructions
/// @param instructions Array to store the loaded instructions
void loadFile(const char *filename, Instruction instructions[]);

#endif
