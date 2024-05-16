#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

// Main memory
uint64_t memory[__UINT16_MAX__];
uint16_t RPC_START = 0x3000;

// Registers: 0-7 general purpose, RPC = program counter, RCNT = number of registers
enum
{
    R0 = 0, R1, R2, R3, R4, R5, R6, R7, RPC, RCNT
};

uint64_t reg[RCNT];

// Opcodes
enum
{
    ADD = 0, // sums two values
    AND, // bitwise And of two values
    NOT, // bitwise complement of a value
    LDR, // loads value into a register
    LD, // loads value from memory location into a register
    STR, // stores register contents into memory
    PUTC, // outputs character from R0
    PUTD, // outputs decimal value from R0
    HALT // ends the program
};

// Convert Opcode to machine code instruction prefix
uint64_t convert_op(char *op)
{
    if (strncmp(op, "ADD", 3) == 0) {
        return 0;
    } else if (strncmp(op, "AND", 3) == 0) {
        return 0x1000000000000000;
    } else if (strncmp(op, "NOT", 3) == 0) {
        return 0x2000000000000000;
    } else if (strncmp(op, "LDR", 3) == 0) {
        return 0x3000000000000000;
    } else if (strncmp(op, "LD", 2) == 0) {
        return 0x4000000000000000;
    } else if (strncmp(op, "STR", 3) == 0) {
        return 0x5000000000000000;
    } else if (strncmp(op, "PUTC", 4) == 0) {
        return 0x6000000000000000;
    } else if (strncmp(op, "PUTD", 4) == 0) {
        return 0x7000000000000000;
    } else if (strncmp(op, "HALT", 4) == 0) {
        return 0x8000000000000000;
    } else {
        puts("Error: Bad Opcode");
        exit(1);
    }
}

// Parse immediate value
uint32_t parse_imm(char *token)
{
    uint32_t x = token[1] - '0';

    for (int i = 2; token[i] != '\0' && token[i] != '\n'; i++) {
        x = x * 10 + (token[i] - '0');
    }
    return x;
}

// Parse register number
uint64_t parse_reg(char *token, int count)
{
    uint64_t r = token[1] - '0';
    uint64_t y = r << (60 - (4 * count));
    return y;
}

// Convert hex to int
uint16_t parse_hex(char *token)
{
    uint16_t result = 0;
    int val, length, i, j;
    length = strlen(token) - 1;
    j = length - 2;

    if (token[length] == '\n') {
        j--;
    }

    for (i = 2; token[i] != '\0' && token[i] != '\n'; i++, j--) {
        if(token[i] >= '0' && token[i] <= '9') {  
            val = token[i] - 48;  
        } else if(token[i] >= 'a' && token[i] <= 'f') {  
            val = token[i] - 97 + 10;  
        } else if(token[i] >= 'A' && token[i] <= 'F') {  
            val = token[i] - 65 + 10;  
        } 
        result += val * pow(16, j);  
    }
    return result;
}

// Convert instruction string to machine code, then store in memory
void parse_line(char line[])
{
    char *token;
    token = strtok(line, " "); 
    int count = 0;
    uint64_t op;
    uint64_t instruction;

    while (token != NULL) {

        if (count == 0) {
            op = convert_op(token);
            instruction = op;
        } else {
            uint16_t opcode = op >> 60;

            switch (opcode) {
                case AND:
                case ADD:
                {
                    if (token[0] == 'R') {
                        uint64_t r = parse_reg(token, count);
                        instruction += r;
                    } else if (token[0] == '#') {
                        uint32_t x = parse_imm(token);
                        uint64_t imm_flag = 0x800000000000; // bit 47
                        instruction += x + imm_flag;                
                    }
                }
                break;
                case NOT:
                {
                    if (token[0] == 'R') {
                        uint64_t r = parse_reg(token, count);
                        instruction += r;
                    }
                }
                break;
                case LDR:
                {
                    if (token[0] == 'R') {
                        uint64_t r = parse_reg(token, count);
                        instruction += r;
                    } else if (token[0] == '#') {
                        uint32_t x = parse_imm(token);
                        uint64_t imm_flag = 0x8000000000000; // bit 51
                        instruction += x + imm_flag;                
                    }
                }
                break;
                case LD:
                case STR:
                {
                    if (token[0] == 'R') {
                        uint64_t r = parse_reg(token, count);
                        instruction += r;
                    } else if (token[0] == '0' && token[1] == 'x') {
                        uint16_t address = parse_hex(token);
                        instruction += address;
                    }
                }
                break;
                default:
                {
                    printf("Error, bad opcode, could not parse line for opcode %ld, full instruction %ld\n", op, instruction);
                    exit(1);
                }
            }
        }
        token = strtok(NULL, " ");
        count++;
    }
    memory[reg[RPC]] = instruction;
}

// Read assembly program into memory
void read_file(char filename[])
{
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        puts("Error: file can't be opened.");
        exit(1);
    }
    char line[50];

    while (fgets(line, 50, file)) {
        parse_line(line);
        reg[RPC]++;
    }

    fclose(file);
}

void execute_op(uint64_t instruction)
{
    uint16_t op = instruction >> 60;

    switch (op) {
        case ADD:
        {
            uint16_t dr = (instruction >> 56) & 0xF;
            uint16_t sr1 = (instruction >> 52) & 0xF;
            uint16_t imm_flag = (instruction >> 47) & 1;

            if (imm_flag) {
                uint32_t val = instruction & 0xFFFFFFFF;
                reg[dr] = reg[sr1] + val;
            } else {
                uint16_t sr2 = (instruction >> 48) & 0xF;
                reg[dr] = reg[sr1] + reg[sr2];
            }
        }
        break;
        case AND:
        {
            uint16_t dr = (instruction >> 56) & 0xF;
            uint16_t sr1 = (instruction >> 52) & 0xF;
            uint16_t imm_flag = (instruction >> 47) & 1;

            if (imm_flag) {
                uint32_t val = instruction & 0xFFFFFFFF;
                reg[dr] = reg[sr1] & val;
            } else {
                uint16_t sr2 = (instruction >> 48) & 0xF;
                reg[dr] = reg[sr1] & reg[sr2];
            }
        }
        break;
        case NOT:
        {
            uint16_t dr = (instruction >> 56) & 0xF;
            uint16_t sr = (instruction >> 52) & 0xF;

            reg[dr] = ~reg[sr]; // 1's complement
        }
        break;
        case LDR:
        {
            uint16_t dr = (instruction >> 56) & 0xF;
            uint16_t imm_flag = (instruction >> 51) & 1;

            if (imm_flag) {
                uint32_t val = instruction & 0xFFFFFFFF;
                reg[dr] = val;
            } else {
                uint16_t sr = (instruction >> 52) & 0xF;
                reg[dr] = reg[sr];
            }
        }
        break;
        case LD:
        {
            uint16_t dr = (instruction >> 56) & 0xF;
            uint16_t address = instruction & 0xFFFF; // last 16 bits

            reg[dr] = memory[address];
        }
        break;
        case STR:
        {
            uint16_t sr = (instruction >> 56) & 0xF;
            uint16_t address = instruction & 0xFFFF;

            memory[address] = reg[sr];
        }
        break;
        case PUTC:
        {
            putchar(reg[R0]);
        }
        break;
        case PUTD:
        {
            printf("%ld", reg[R0]);
        }
        break;
        case HALT:
        {
            putchar('\n');
            exit(0);
        }
        default:
        {
            printf("Error, bad opcode, could not execute opcode %d, full instruction %ld\n", op, instruction);
            exit(1);
        }          
    }
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("vm [asm-file]");
        exit(1);
    }
    reg[RPC] = RPC_START;
    read_file(argv[1]);
    reg[RPC] = RPC_START;

    while (1) {
        uint64_t instruction = memory[reg[RPC]];
        execute_op(instruction);
        reg[RPC]++;

    }
    return 0;
}