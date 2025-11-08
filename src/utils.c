/// @file utils.c
/// @brief Implements utility functions for emulator state management and help display.
///
/// This file defines the implementation of functions declared in utils.h,
/// including saving/loading the emulator state to a binary file and printing
/// command-line usage information.

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

bool saveState(const uint8_t *memory, const z80 *cpu) {
    FILE *file = fopen("save.bin", "wb");
    if(!file) {
        perror("Error while opening save file");
        return false;
    }

    fwrite(memory, sizeof(uint8_t), 0x10000, file);
    
    fwrite(cpu, sizeof(z80), 1, file);
    
    fclose(file);

    return true;
}

bool loadState(uint8_t *memory, z80 *cpu) {
    FILE *file = fopen("save.bin", "rb");
    if(!file) {
        perror("Error while opening save file");
        return false;
    }

    fread(memory, sizeof(uint8_t), 0x10000, file);
    fread(cpu, sizeof(z80), 1, file);
    
    fclose(file);
    return true;
}


void printHelp() {
    printf("Usage: zasm [OPTIONS]\n");
    printf("\nGestisce l'emulatore Z80.\n\n");
    printf("  -f <file>           The file to emulate\n");
    printf("  -r                  Resets the state of the CPU and memory\n");
    printf("  -c, --view-cpu      Shows the CPU state\n");
    printf("  -m, --view-memory   Shows the memory state\n");
    printf("  --view-state        Shows both the CPU and memory state\n");
    printf("  -h, --help          Mostra questo messaggio di aiuto\n");
    
    return;
}

bool fileExist(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if(!file) {
        return false;
    }

    fclose(file);
    return true;
}

void printRegisters() {
	printf("REGISTERS STATUS:\n");
	printf("A: [%02X]\n", cpu.A);
	printf("B: [%02X]\n", cpu.B);
	printf("C: [%02X]\n", cpu.C);
	printf("D: [%02X]\n", cpu.D);
	printf("E: [%02X]\n", cpu.E);
	printf("H: [%02X]\n", cpu.H);
	printf("L: [%02X]\n", cpu.L);
	printf("PC: [%04X]\n", cpu.PC);

	return;
}

void printFlags() {
	printf("FLAGS STATUS:\n");
	printf("SF: [%d]\n", cpu.F.SF);
	printf("ZF: [%d]\n", cpu.F.ZF);
	printf("YF: [%d]\n", cpu.F.YF);
	printf("HF: [%d]\n", cpu.F.HF);
	printf("XF: [%d]\n", cpu.F.XF);
	printf("PF: [%d]\n", cpu.F.PF);
	printf("NF: [%d]\n", cpu.F.NF);
	printf("CF: [%d]\n", cpu.F.CF);
}

void printMemory() {
    printf("      ");
    for (int i = 0; i < 16; i++) {
        printf("%02X ", i);
    }

    printf("\n      ");

    for (int i = 0; i < 23; i++) {
        printf("--");
    }

    printf("-\n");

    for (uint32_t addr = 0; addr < 0x10000; addr += 16) {
        bool allZero = true;

        // Check if the row is all zeroes
        for (int i = 0; i < 16; i++) {
            if (memory[addr + i] != 0x00) {
                allZero = false;
                break;
            }
        }

        if (allZero) continue;

        // print address
        printf("%04X: ", addr);

        // print byte in hex
        for (int i = 0; i < 16; i++) {
            printf("%02X ", memory[addr + i]);
        }

        printf(" ");

        // print the line in ASCII char
        for (int i = 0; i < 16; i++) {
            uint8_t c = memory[addr + i];
            printf("%c", isprint(c) ? c : '.');
        }

        printf("\n");
    }
}

void str_to_upper(char *str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

bool ends_with(char str[], const char c) {
	int pos = strlen(str);

	if(str[pos-1] == '\n') {
		str[pos-1] = '\0';
	}
	pos = strlen(str);

	if(str[pos-1] == c) return true;
	
	return false;
}

char *instTypeToString(InstructionType type) {
    switch (type) {
        case IST_LD:    return "IST_LD";
        case IST_ADD:   return "IST_ADD";
        case IST_SUB:   return "IST_SUB";
        case IST_AND:   return "IST_AND";
        case IST_OR:    return "IST_OR";
        case IST_XOR:   return "IST_XOR";
        case IST_CP:    return "IST_CP";
        case IST_INC:   return "IST_INC";
        case IST_DEC:   return "IST_DEC";
        case IST_HALT:  return "IST_HALT";
        case IST_NOP:   return "IST_NOP";
        case IST_ERR:   return "IST_ERR";
        default:        return "UNKNOWN_INSTRUCTION";
    }
}

void stripParenthesis(char *str) {
    char *src = str;  // read
    char *dst = str;  // write

    while (*src) {
        if (*src != '(' && *src != ')') {
            *dst = *src;  // copies only if not parenthesis
            dst++;
        }
        src++;
    }

    *dst = '\0';
}