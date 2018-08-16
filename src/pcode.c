#include <stdio.h>
#include <string.h>
#include "pcode.h"

int findBase(int);      // Find base n levels down
void callOperation();   // Executes instruction 'OPR 0 X', where X is an operation between 0 and 13
void printStack();      // Prints current stack state
void printRegisters();  // Prints current registers state
void printNextInstruction();

const char *INSTRUCTIONS_STR[] = {"LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC"};

int p;  // Program register
int b;  // Base register
int t;  // Top-stack register
int *stack;
const Instruction *currIns; // Instruction currently being executed

int parseSource(FILE *stream, Instruction *code, char *msg) {
    char statement[255];
    int count = 0;

    while (fgets(statement, 255, stream) != NULL) {
        if (!strToInstruction(statement, &code[count])) {
            if (msg != NULL)
                sprintf(msg, "Line %d, unknown instruction:\n\t%s", count + 1, statement);

            return 0;
        }

        count++;
    }

    return count;
}

void execute(const Instruction *code, int *stackData) {
    stack = stackData;
    p = 0;
    b = 0;
    t = -1;
    
    do {
        currIns = &code[p++];
        printStack();
        printNextInstruction();

        switch (currIns->f) {
            case LIT:
                stack[++t] = currIns->a;
                break;

            case OPR:
                callOperation();
                break;

            case LOD:
                stack[++t] = stack[findBase(currIns->l) + currIns->a];
                break;

            case STO:
                stack[findBase(currIns->l) + currIns->a] = stack[t--];
                break;

            case CAL:
                stack[t + 1] = findBase(currIns->l);
                stack[t + 2] = b;
                stack[t + 3] = p;
                b = t + 1;
                p = currIns->a;
                break;

            case INT:
                t += currIns->a;
                break;

            case JMP:
                p = currIns->a;
                break;

            case JPC:
                if (stack[t] == 0)
                    p = currIns->a;
                t--;
                break;
            
            default:
                return;
        }
    } while (p > 0);
}

void callOperation() {
    switch (currIns->a) {
        case RET:
            t = b - 1;
            p = stack[t + 3];
            b = stack[t + 2];
            break;

        case NEG:
            stack[t] *= -1;
            break;

        case ADD:
            t--;
            stack[t] += stack[t + 1];
            break;

        case SUB:
            t--;
            stack[t] -= stack[t + 1];
            break;

        case MUL:
            t--;
            stack[t] *= stack[t + 1];
            break;

        case DIV:
            t--;
            stack[t] /= stack[t + 1];
            break;

        case ODD:
            stack[t] = stack[t] % 2 > 0;
            break;

        case EQU:
            t--;
            stack[t] = stack[t] == stack[t + 1];
            break;

        case NEQ:
            t--;
            stack[t] = stack[t] != stack[t + 1];
            break;

        case LSS:
            t--;
            stack[t] = stack[t] < stack[t + 1];
            break;

        case GEQ:
            t--;
            stack[t] = stack[t] >= stack[t + 1];
            break;

        case GTR:
            t--;
            stack[t] = stack[t] > stack[t + 1];
            break;

        case LEQ:
            t--;
            stack[t] = stack[t] <= stack[t + 1];
            break;

        default:
            return;
    }
}

int strToInstruction(const char *str, Instruction *ins) {
    char fctStr[4];
    int i;

    sscanf(str, "%s %d %d\n", fctStr, &(ins->l), &(ins->a));

    for (i = 0; i < 8; i++)
        if (strcmp(fctStr, INSTRUCTIONS_STR[i]) == 0) {
            ins->f = i;
            return 1;
        }

    return 0;
}

void instructionToStr(const Instruction *ins, char *str) {
    sprintf(str, "%s %d %d\n", INSTRUCTIONS_STR[ins->f], ins->l, ins->a);
}

int findBase(int level) {
    int b2 = b;

    while (level > 0) {
        b2 = stack[b2];
        level--;
    }

    return b2;
}

void printStack() {
    int i = t > b + 2 ? t : b + 2;

    puts("---------------------------");

    for (; i >= 0; i--) {
        printf("[%d]:\t%d", i, stack[i]);

        if (i == t)
            printf(" <-- Top");

        if (i == b)
            printf(" <-- Base");

        puts("");
    }
}

void printNextInstruction() {
    char strIns[255];

    instructionToStr(currIns, strIns);
    printf("\nNext instruction: %s\n", strIns);
}

void printRegisters() {
    printf("\nProg: %d\tBase: %d\tTop: %d\n", p, b, t);
}