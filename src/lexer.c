#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "utils.h"
#include "lexer.h"

InstructionType handleInstType(char *instType) {
    size_t len = strlen(instType);
    if(len > 0 && instType[len-1] == '\n') {
        instType[len-1] = '\0';
    }

    if (strcmp(instType, "LD") == 0) {
        return IST_LD;
    } else if (strcmp(instType, "ADD") == 0) {
        return IST_ADD;
    } else if (strcmp(instType, "HALT") == 0) {
        return IST_HALT;
    } else if (strcmp(instType, "NOP") == 0) {
        return IST_NOP;
    } else if(strcmp(instType, "INC") == 0) {
        return IST_INC;
    } else if(strcmp(instType, "DEC") == 0) {
        return IST_DEC;
    } else if(strcmp(instType, "SUB") == 0) {
        return IST_SUB;
    } else if(strcmp(instType, "ADC") == 0) {
        return IST_ADC;
    } else if(strcmp(instType, "SBC") == 0) {
        return IST_SBC;
    } else if(strcmp(instType, "AND") == 0) {
        return IST_AND;
    } else if(strcmp(instType, "XOR") == 0) {
        return IST_XOR;
    } else if(strcmp(instType, "OR") == 0) {
        return IST_OR;
    } else if(strcmp(instType, "DJNZ") == 0) {
        return IST_DJNZ;
    } else if(strcmp(instType, "JP") == 0) {
        return IST_JP;
    } else if(strcmp(instType, "JR") == 0) {
        return IST_JR;
    } else if(strcmp(instType, "CP") == 0) {
        return IST_CP;
    } else if(strcmp(instType, "CCF") == 0) {
        return IST_CCF;
    } else if(strcmp(instType, "SCF") == 0) {
        return IST_SCF;
    } else if(strcmp(instType, "NEG") == 0) {
        return IST_NEG;
    } else {
        return IST_ERR;
    }
}

Instruction parseLine(char line[], uint16_t IC) {
    Instruction instruction;
    char * token;

    instruction.IC = IC;

    // skips to the first char that isnt a empty (space or tab)
    while (*line == ' ' || *line == '\t') {
        line++;
    }

    token = strtok(line, "\t ");
    str_to_upper(token);
    instruction.instType = handleInstType(token);


    // inizializza arg1 e arg2
    instruction.arg1[0] = '\0';
    instruction.arg2[0] = '\0';

    int i = 0; // cont args
    while ((token = strtok(NULL, " ,")) != NULL) {
        // rimuovi spazi iniziali
        while (*token == ' ') token++;

        if (i == 0) { // primo arg
            strcpy(instruction.arg1, token);
        } else if (i == 1) { // secondo arg
            strcpy(instruction.arg2, token);
        }
        i++;
    }

    return instruction;
}

void loadFile(const char *filename, Instruction instructions[]) {
    FILE * src = fopen(filename, "rt");

    if(!src) return;

    char buffer[8192];

    uint16_t istCounter = 0;
    while(fgets(buffer, sizeof(buffer), src) != NULL) {
        instructions[istCounter] = parseLine(buffer, istCounter);
        istCounter++;
    }

    fclose(src);
}
