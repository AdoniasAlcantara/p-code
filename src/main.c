#include <stdio.h>
#include "pcode.h"

#define CODE_MAX 500
#define DATA_SIZE 500

int main(int argc, char **argv) {
    Instruction code[CODE_MAX]; // Instruction execution sequence
    int data[DATA_SIZE] = {0};  // Data store
    int lines;                  // Instruction line count

    FILE *sourceFile;
    char message[255];

    if (argc < 2) {
        fprintf(stderr, "Missing argument: program_file\n");
        return 1;
    }

    sourceFile = fopen(argv[1], "r");

    if (sourceFile == NULL) {
        fprintf(stderr, "File %s doesn't exist\n", argv[1]);
        return 1;
    }

    if ((lines = parseSource(sourceFile, code, message)) == 0) {
        fprintf(stderr, "Error while parsing file. %s\n", message);
        return 1;
    }
    
    fclose(sourceFile);
    printf("Parsing ok! %d lines found.\n", lines);
    execute(code, data);

    return 0;
}