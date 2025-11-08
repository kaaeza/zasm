#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "z80.h"
#include "utils.h"
#include "lexer.h"
#include "emulator.h"

uint8_t *getReg8Bit(const char reg) {
    switch (reg) {
    case 'A':
        return &cpu.A;
    case 'B':
        return &cpu.B;
    case 'C':
        return &cpu.C;
    case 'D':
        return &cpu.D;
    case 'E':
        return &cpu.E;
    case 'H':
        return &cpu.H;
    case 'L':
        return &cpu.L;
    }

    return NULL;
}

uint8_t *handleArg(char *arg) {
    static uint8_t immNumber = 0; // immediate number
    uint8_t *a;
    if(arg[0] == '(') {
        stripParenthesis(arg);

        if(ends_with(arg, 'h')) {
            uint8_t val = (uint8_t) strtol(arg, NULL, 16);
            return &memory[val];
        } else {
            a = getReg8Bit(arg[0]);
            
            return &memory[*a];
        }
    }
    
    if(ends_with(arg, 'h')) {
        immNumber = (uint8_t) strtol(arg, NULL, 16);
        return &immNumber;
    } else {
        return getReg8Bit(arg[0]);
    }

    return NULL;
}

void execFile(Instruction instructions[]) {
    bool interrupted = false;
    uint8_t *dest;
    uint8_t *src;

    while (!interrupted) {
        Instruction currentInstruction = instructions[cpu.PC];

        switch (currentInstruction.instType) {
        /* ---------------------------------------
         * LOAD (LD) INSTRUCTION
         * --------------------------------------- */

        case IST_LD: {
            dest = handleArg(currentInstruction.arg1);
            src = handleArg(currentInstruction.arg2);

            bool isDestMemory = (dest >= memory) && (dest < memory + 0x10000);
            if(isDestMemory) {
                fprintf(stderr, "Line %04X: [ERROR] Unrecognized instruction/syntax. 'LD (<arg>), <arg>'\n", cpu.PC);
                exit(1);
            }

            *dest = *src;
            break;
        }

        /* ---------------------------------------
         * 8-BIT ARITHMETIC GROUP
         * --------------------------------------- */

        case IST_INC: {
            src = handleArg(currentInstruction.arg1);
            (*src)++;
            cpu.F.ZF = (*src == 0); /* Set Zero flag if result is 0 */
            break;
        }

        case IST_DEC: {
            src = handleArg(currentInstruction.arg1);
            (*src)--;
            cpu.F.ZF = (*src == 0);
            break;
        }

        case IST_ADD: {
            src = handleArg(currentInstruction.arg1);

            cpu.A += *src;
            cpu.F.ZF = (cpu.A == 0);
            break;
        }
        
        case IST_SUB: {
            src = handleArg(currentInstruction.arg1);
            
            cpu.A -= *src;
            cpu.F.ZF = (cpu.A == 0);
            break;
        }

        /* ---------------------------------------
         * 8-BIT LOGICAL GROUP
         * --------------------------------------- */

        case IST_CP: {
            src = handleArg(currentInstruction.arg1);

            cpu.F.ZF = !(cpu.A == *src);
            break;
        }

        case IST_AND: {
            src = handleArg(currentInstruction.arg1);
            cpu.A &= *src;
            break;
        }

        case IST_OR: {
            src = handleArg(currentInstruction.arg1);

            cpu.A |= *src;
            break;
        }

        case IST_XOR: {
            src = handleArg(currentInstruction.arg1);

            cpu.A ^= *src;
            break;
        }
        
        /* ---------------------------------------
         * JUMP GROUP
         * --------------------------------------- */

        case IST_JP: {
            uint16_t pcJump = 0;
            if (strlen(currentInstruction.arg2) > 0) {
                pcJump = (uint16_t)strtol(currentInstruction.arg2, NULL, 16);
                char condition[4];
                strncpy(condition, currentInstruction.arg1, sizeof(condition) - 1);
                condition[sizeof(condition) - 1] = '\0';

                if (strcmp(condition, "NZ") == 0) { /* Non Zero */
                    if (!cpu.F.ZF) {
                        cpu.PC = pcJump;
                        cpu.PC--;
                    }
                } else if (strcmp(condition, "Z") == 0) { /* Zero */
                    if (cpu.F.ZF) {
                        cpu.PC = pcJump;
                        cpu.PC--;
                    }
                } else if (strcmp(condition, "NC") == 0) { /* Non Carry */
                    if (!cpu.F.CF) {
                        cpu.PC = pcJump;
                        cpu.PC--;
                    }
                } else if (strcmp(condition, "C") == 0) { /* Carry */
                    if (cpu.F.CF) {
                        cpu.PC = pcJump;
                        cpu.PC--;
                    }
                } else if (strcmp(condition, "PO") == 0) { /* Parity Odd */
                    if (!cpu.F.PF) {
                        cpu.PC = pcJump;
                        cpu.PC--;
                    }
                } else if (strcmp(condition, "PE") == 0) { /* Parity Even */
                    if (cpu.F.PF) {
                        cpu.PC = pcJump;
                        cpu.PC--;
                    }
                }
            } else {
                pcJump = strtol(currentInstruction.arg1, NULL, 16);
                cpu.PC = pcJump;
                // prevent the Program counter to be +1, so the PC will be the absolute number of the inst to jump to
                cpu.PC--;
            }
            break;
        }

        case IST_JR: {
            uint16_t pcJump = 0;
            if (strlen(currentInstruction.arg2) > 0) {
                pcJump = (uint16_t)strtol(currentInstruction.arg2, NULL, 16);
                char condition[4];
                strncpy(condition, currentInstruction.arg1, sizeof(condition) - 1);
                condition[sizeof(condition) - 1] = '\0';

                if (strcmp(condition, "NZ") == 0) { /* Non Zero */
                    if (!cpu.F.ZF) {
                        cpu.PC -= pcJump;
                        cpu.PC--;
                    }
                } else if (strcmp(condition, "Z") == 0) { /* Zero */
                    if (cpu.F.ZF) {
                        cpu.PC -= pcJump;
                        cpu.PC--;
                    }
                } else if (strcmp(condition, "NC") == 0) { /* Non Carry */
                    if (!cpu.F.CF) {
                        cpu.PC -= pcJump;
                        cpu.PC--;
                    }
                } else if (strcmp(condition, "C") == 0) { /* Carry */
                    if (cpu.F.CF) {
                        cpu.PC -= pcJump;
                        cpu.PC--;
                    }
                } else if (strcmp(condition, "PO") == 0) { /* Parity Odd */
                    if (!cpu.F.PF) {
                        cpu.PC -= pcJump;
                        cpu.PC--;
                    }
                } else if (strcmp(condition, "PE") == 0) { /* Parity Even */
                    if (cpu.F.PF) {
                        cpu.PC -= pcJump;
                        cpu.PC--;
                    }
                }
            } else {
                pcJump = strtol(currentInstruction.arg1, NULL, 16);
                cpu.PC -= pcJump;
                // prevent the Program counter to be +1, so the PC will be the absolute number of the inst to jump to
                cpu.PC--;
            }
            break;
        }

        case IST_DJNZ: {
            src = handleArg(currentInstruction.arg1);

            cpu.B--;

            if(cpu.B != 0) {
                cpu.PC += *src;
                cpu.PC--;
            }

            break;
        }
        /* ---------------------------------------
         * OTHER INSTRUCTIONS
         * --------------------------------------- */

        case IST_NOP:
            break;

        case IST_HALT:
            interrupted = true;
            // prevent the Program counter to be +1 and considers HALT the last instruction
            cpu.PC--;
            break;

        case IST_ERR: {
            fprintf(stderr, "Line %04X: [ERROR] Unrecognized instruction/syntax.\n", cpu.PC);
            exit(1);
        }

        default:
            break;
        }

        
        if (debugActive) {
            printf("[DEBUG]: Executed %s\n", instTypeToString(currentInstruction.instType));
        }
        // Increment the Program Counter after every instruction
        cpu.PC++;
    }
}