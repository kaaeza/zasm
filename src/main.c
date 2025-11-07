#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "emulator.h"
#include "z80.h"
#include "utils.h"
#include "lexer.h"

// defined in z80.h to use in every file by including it
uint8_t memory[0x10000];
z80 cpu;
bool debugActive;
int main(int argc, char *argv[]) {
    // loads the previously saved memory and cpu
    loadState(memory, &cpu);
    // flag switch
    int opt;
    // file to open (if there is one)
    char filename[FILENAME_MAX] = "";
    // cli flags
    bool showCpu = false;
    bool showMem = false;
    bool showFullState = false;

    const struct option long_options[] = {
        {"debug",      no_argument,       0, 'd'},
        {"view-cpu",   no_argument,       0, 'c'},
        {"view-memory",no_argument,       0, 'm'},
        {"view-state", no_argument,       0,  1 },
        {"help",       no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };

    if (argc < 2) {
        printf("zasm: missing flag or file.\n\n");
        printf("Try 'zasm -h' or 'zasm --help' for more information.\n\n");
        return 1;
    }

    while ((opt = getopt_long(argc, argv, "f:rcmhd", long_options, NULL)) != -1) {
        switch (opt) {
        case 'd':
            debugActive = true;
            break;
        case 'f':
            strcpy(filename, optarg);

            if(!fileExist(filename)) {
                printf("Error: File \"%s\" doesn't exist", filename);
                return 1;
            }
            break;

        case 'r':
            memset(&cpu, 0, sizeof(cpu));
            memset(&memory, 0, sizeof(memory));
            printf("CPU registers and memory have been reset.\n");
            break;

        case 'c':
            showCpu = true;
            break;

        case 'm':
            showMem = true;
            break;

        case 1: // --view-state
            showFullState = true;
            break;

        case 'h':
            printHelp();
            return 0;
        
        case '?':  // flag error
            if (optopt == 'f') {
                fprintf(stderr, "Error: the option -f requires an argument <filename>.\n");
            } else {
                fprintf(stderr, "Error: Unknown or missing option.\n");
            }
            return 1;

        default:
            printf("Unknown flag: %c\n\n", opt);
            printHelp();
            return 1;
        }
    }

    if(strlen(filename) > 0) {
        // TODO: start the real emulator
        Instruction instructions[4096];
        loadFile(filename, instructions);
        execFile(instructions);
    }

    saveState(memory, &cpu);

    if(showCpu) {
        printRegisters();
        printFlags();
    }

    if(showMem) {
        printMemory();
    }

    if(showFullState) {
        printRegisters();
        printf("\n");
        printFlags();
        printMemory();
    }
    return 0;
}
