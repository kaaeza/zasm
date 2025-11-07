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

void execFile(Instruction instructions[]) {
    bool interrupted = false;

    while (!interrupted) {
        Instruction currentInstruction = instructions[cpu.PC];
        uint8_t *reg;

        switch (currentInstruction.instType) {
        /* ---------------------------------------
         * LOAD (LD) INSTRUCTION
         * --------------------------------------- */

        case IST_LD: {
            /* If the first argument is a memory cell (e.g. "2Ah") */
            if (ends_with(currentInstruction.arg1, 'h')) {
                unsigned long _dest = strtol(currentInstruction.arg1, NULL, 16);
                uint8_t dest = (uint8_t)_dest;

                /* If the second argument is also a memory address */
                if (ends_with(currentInstruction.arg2, 'h')) {
                    unsigned long _num = strtol(currentInstruction.arg2, NULL, 16);
                    uint8_t num = (uint8_t)_num;
                    memory[dest] = num;
                }
                /* If the second argument is a register */
                else {
                    uint8_t *src = getReg8Bit(currentInstruction.arg2[0]);
                    memory[dest] = *src;
                }
            }
            /* If the first argument is a register */
            else {
                uint8_t *dest = getReg8Bit(currentInstruction.arg1[0]);

                /* If the second argument is an immediate hex value */
                if (ends_with(currentInstruction.arg2, 'h')) {
                    unsigned long _num = strtol(currentInstruction.arg2, NULL, 16);
                    uint8_t num = (uint8_t)_num;
                    *dest = num;
                }
                /* If the second argument is a register */
                else {
                    uint8_t *src = getReg8Bit(currentInstruction.arg2[0]);
                    *dest = *src;
                }
            }

            break;
        }

        /* ---------------------------------------
         * 8-BIT ARITHMETIC GROUP
         * --------------------------------------- */

        case IST_INC: {
            reg = getReg8Bit(currentInstruction.arg1[0]);
            (*reg)++;
            cpu.F.ZF = (*reg == 0); /* Set Zero flag if result is 0 */
            break;
        }

        case IST_DEC: {
            reg = getReg8Bit(currentInstruction.arg1[0]);
            (*reg)--;
            cpu.F.ZF = (*reg == 0);
            break;
        }

        case IST_ADD: {
            uint8_t add1 = 0;
            uint8_t add2 = 0;

            // If the first argument is a memory cell
            if (ends_with(currentInstruction.arg1, 'h')) {
                unsigned long _temp = strtol(currentInstruction.arg1, NULL, 16);
                add1 = memory[(uint8_t)_temp];

                // If the second argument is also a memory cell
                if (ends_with(currentInstruction.arg2, 'h')) {
                    unsigned long _num = strtol(currentInstruction.arg2, NULL, 16);
                    add2 = memory[(uint8_t)_num];

                } else { // If the second argument is a register
                    uint8_t *src = getReg8Bit(currentInstruction.arg2[0]);
                    add2 = *src;
                }
            } else { // If the first argument is a register
                reg = getReg8Bit(currentInstruction.arg1[0]);
                add1 = *reg;

                // Second argument is a memory cell
                if (ends_with(currentInstruction.arg2, 'h')) {
                    unsigned long _num = strtol(currentInstruction.arg2, NULL, 16);
                    add2 = memory[(uint8_t)_num];

                } else { // Second argument is a register
                    reg = getReg8Bit(currentInstruction.arg2[0]);
                    add2 = *reg;
                }
            }

            cpu.A = add1 + add2;
            cpu.F.ZF = (cpu.A == 0);
            break;
        }
        
        case IST_SUB: {
            uint8_t sub1 = 0;
            uint8_t sub2 = 0;

            // If the first argument is a memory cell
            if (ends_with(currentInstruction.arg1, 'h')) {
                unsigned long _temp = strtol(currentInstruction.arg1, NULL, 16);
                sub1 = memory[(uint8_t)_temp];

                // If the second argument is also a memory cell
                if (ends_with(currentInstruction.arg2, 'h')) {
                    unsigned long _num = strtol(currentInstruction.arg2, NULL, 16);
                    sub2 = memory[(uint8_t)_num];

                } else { // If the second argument is a register
                    uint8_t *src = getReg8Bit(currentInstruction.arg2[0]);
                    sub2 = *src;
                }
            } else { // If the first argument is a register
                reg = getReg8Bit(currentInstruction.arg1[0]);
                sub1 = *reg;

                // Second argument is a memory cell
                if (ends_with(currentInstruction.arg2, 'h')) {
                    unsigned long _num = strtol(currentInstruction.arg2, NULL, 16);
                    sub2 = memory[(uint8_t)_num];

                } else { // Second argument is a register
                    reg = getReg8Bit(currentInstruction.arg2[0]);
                    sub2 = *reg;
                }
            }

            cpu.A = sub1 - sub2;
            cpu.F.ZF = (cpu.A == 0);
            break;
        }

        /* ---------------------------------------
         * 8-BIT LOGICAL GROUP
         * --------------------------------------- */

        case IST_CP: {
            if (ends_with(currentInstruction.arg1, 'h')) {
                uint8_t val = strtol(currentInstruction.arg1, NULL, 16);
                /* ZF = 0 if A == val (Z80 logic) */
                cpu.F.ZF = !(cpu.A == val);
            } else {
                reg = getReg8Bit(currentInstruction.arg1[0]);
                cpu.F.ZF = !(cpu.A == *reg);
            }

            break;
        }

        case IST_AND: {
            uint8_t val = 0;
            if(ends_with(currentInstruction.arg1, 'h')) {
                val = (uint8_t) strtol(currentInstruction.arg1, NULL, 16);
            } else {
                val = *(getReg8Bit(currentInstruction.arg1[0]));
            }

            cpu.A &= val;
            break;
        }

        case IST_OR: {
            uint8_t val = 0;
            if(ends_with(currentInstruction.arg1, 'h')) {
                val = (uint8_t) strtol(currentInstruction.arg1, NULL, 16);
            } else {
                val = *(getReg8Bit(currentInstruction.arg1[0]));
            }

            cpu.A |= val;
            break;
        }

        case IST_XOR: {
            uint8_t val = 0;
            if(ends_with(currentInstruction.arg1, 'h')) {
                val = (uint8_t) strtol(currentInstruction.arg1, NULL, 16);
            } else {
                val = *(getReg8Bit(currentInstruction.arg1[0]));
            }

            cpu.A ^= val;
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

        case IST_ERR:
            interrupted = true;
            fprintf(stderr, "error: unrecognized instruction at line %d\n", cpu.PC);
            break;
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