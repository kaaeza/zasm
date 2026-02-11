#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "utils.h"
#include "lexer.h"

typedef struct {
    const char *name;
    InstructionType type;
} InstMap;

static const InstMap inst_map[] = {
    {"LD",   IST_LD},
    {"ADD",  IST_ADD},
    {"HALT", IST_HALT},
    {"NOP",  IST_NOP},
    {"INC",  IST_INC},
    {"DEC",  IST_DEC},
    {"SUB",  IST_SUB},
    {"ADC",  IST_ADC},
    {"SBC",  IST_SBC},
    {"AND",  IST_AND},
    {"XOR",  IST_XOR},
    {"OR",   IST_OR},
    {"DJNZ", IST_DJNZ},
    {"JP",   IST_JP},
    {"JR",   IST_JR},
    {"CP",   IST_CP},
    {"CCF",  IST_CCF},
    {"SCF",  IST_SCF},
    {"NEG",  IST_NEG}
};

InstructionType handleInstType(char *instType) {
    size_t len = strlen(instType);
    if(len > 0 && instType[len-1] == '\n') {
        instType[len-1] = '\0';
    }

    for(size_t i = 0; i < sizeof(inst_map) / sizeof(inst_map[0]); i++) {
        if(strcmp(instType, inst_map[i].name) == 0) {
            return inst_map[i].type;
        }
    }

    return IST_ERR;
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
