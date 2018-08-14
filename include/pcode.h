#ifndef PCODE_H
#define PCODE_H
/* 
 * Author:  Adonias A. S. Neto
 * Version: 0.1
 * 
 * This header provides types and functions to parse and 
 * executing source code of the P-Code Machine.
 */

/*
 * P-Code's instruction set.
 * These are instructions related to memory moving,
 * flow control and a subset of operations.
 */
typedef enum {
    LIT,    // 0 Load constant
    OPR,    // 1 Execute operation
    LOD,    // 2 Load variable
    STO,    // 3 Store variable
    CAL,    // 4 Call procedure
    INT,    // 5 Increment stack-top register
    JMP,    // 6 Jump
    JPC     // 7 Jump conditional
} Fct;

/*
 * Set of arithmetic and relational operations.
 * Operations are invoked through the OPR instruction.
 */
typedef enum {
    RET,    // 0 Return
    NEG,    // 1 Two's complement negation
    ADD,    // 2 Add
    SUB,    // 3 Subtract
    MUL,    // 4 Multiply
    DIV,    // 5 Divide
    ODD,    // 6 Odd
    EQU = 8,// 8 Equal
    NEQ,    // 9 Not equal
    LSS,    // 10 Less than
    GEQ,    // 11 Greater or equal
    GTR,    // 12 Greater
    LEQ     // 13 Less or equal
} Operation;

/*
 * An Instruction type consists of a Fct and two integers values,
 * which respectively represent instruction code, level and
 * argument/address.
 */
typedef struct {
    Fct f;  // Instruction code
    int l;  // Level
    int a;  // Argument/address
} Instruction;

/*
 * Executes the instruction sequence provided by the code array.
 * The resulting data stack from execution is placed into data
 * parameter.
 */
void execute(const Instruction *code, int *data);

/*
 * Converts the given string into an Instruction type. The string
 * is a single P-Code statement line.
 * Returns 1 if conversion succeeds. Otherwise, return 0.
 */ 
int strToInstruction(const char *str, Instruction *ins);

/*
 * Produces a string representing the Instruction. Useful for 
 * human readable formatting.
 */
void instructionToStr(const Instruction *ins, char *str);

/*
 * Parses multiple source code lines from input stream and
 * generates the Instruction array. Optionally, error message
 * can be catched through message parameter.
 * Returns the line count found or 0 if an error occurred.
 */
int parseSource(FILE *stream, Instruction *code, char *message);

#endif