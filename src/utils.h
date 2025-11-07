/// @file utils.h
/// @brief Declarations for emulator utility functions
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "z80.h"
#include "lexer.h"

/// @brief Saves the state of the memory and the cpu in a binary file
/// @param memory The memory to save
/// @param cpu The cpu to save
/// @return True if there wasn't any error, False if there was.
bool saveState(const uint8_t *memory, const z80 *cpu);

/// @brief Loads the previous saved state of the memory and cpu
/// @param memory The memory in which to load the previous state
/// @param cpu The cpu in which to load the previous state
/// @return True if there wasn't any error, False if there was.
bool loadState(uint8_t *memory, z80 *cpu);

/// @brief Prints the emulator usage and available flags.
void printHelp();

/// @brief Checks if a file exists.
/// @param filename Filename path to the file to check.
/// @return True if the file exists, False otherwise.
bool fileExist(const char *filename);

/// @brief Prints the CPU registers
void printRegisters();

/// @brief Prints the CPU flags
void printFlags();

/// @brief Print the memory
void printMemory();

/// @brief Converts all characters in a string to uppercase.
/// @param str Pointer to a null-terminated string to be converted.
void str_to_upper(char *str);

/// @brief Checks if a string ends with a specific character.
/// @param str Null-terminated string to check.
/// @param c Character to compare against the last character of the string.
/// @return True if the string ends with the given character, False otherwise.
bool ends_with(char str[], const char c);

// DEBUG FUNCTIONS

/// @brief 
/// @param type 
/// @return 
char *instTypeToString(InstructionType type);
#endif