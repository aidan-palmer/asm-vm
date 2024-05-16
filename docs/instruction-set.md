## Instruction Set

### ADD
Adds a 32-bit unsigned integer or value held in a register to the value stored in the first source register, then stores the sum in the destination register.
Has two modes, determined by bit 47. If bit 47 == 1, an immediate 32-bit unsigned integer is used. The number must have the prefix # for it to be parsed correctly.

Examples: ADD R0, R1, #42   -- adds the value 42 to the value held in register R1, then stores the sum in R0.

          ADD R1, R0, R2    -- adds the values held in R0 and R2 together, then stores the sum in R1.

Encoding: bits 63-60; bits 59-56; bits 55-52;   bits 51-48;         bit 47; bits 46-32; bits 31-0

          Opcode        DR          SR1      SR2 (if imm flag 0)   Imm flag  Unused      Unsigned integer

          0000                                                  1 for immediate, 0 for register

### AND
Calculates the bitwise logical And of the values in two registers, or an immediate value and a register's value.

Examples: AND R0, R1, #42   -- And's the value 42 with the value held in register R1, then stores the result in R0.

          AND R1, R0, R2    -- And's the values held in R0 and R2 together, then stores the result in R1.

Encoding: bits 63-60; bits 59-56; bits 55-52;   bits 51-48;         bit 47; bits 46-32; bits 31-0

          Opcode        DR          SR1      SR2 (if imm flag 0)   Imm flag  Unused      Unsigned integer

          0001                                                 1 for immediate, 0 for register

### NOT
Calculates the bitwise 1's complement (each bit is flipped) of a register's value, then stores the result in the destination register.

Example: NOT R1, R2    -- the complement of the value held in R2 is calculated and the result is stored in R1

Encoding: bits 63-60; bits 59-56; bits 55-52; bits 51-0;

          Opcode        DR            SR        Unused 

          0010

### LDR
Loads a value from one register to another, or loads an immediate value into a register.

Examples: LDR R0, R1  -- loads the value held in R1 into R0, overwriting R0's previous contents

          LDR R1, #42 -- loads 42 into R1, overwriting R1's previous contents

Encoding: bits 63-60; bits 59-56;   bits 55-52;         bit 51; bits 50-32; bits 31-0

          Opcode        DR       SR(if imm flag 0)    Imm flag  Unused      Unsigned integer

          0011

### LD
Loads the value from a memory location into a register.

Example: LD R1, 0x3FFF   -- loads the value held at hexadecimal memory address 3FFF into register R1.

Encoding: bits 63-60; bits 59-56; bits 55-16; bits 15-0

          Opcode        DR         Unused      Memory address

          0100

### STR
Stores a register's contents into a memory location. Inverse operation of LD.

Example: STR R1, 0x3FFF   -- stores the contents of R1 into memory location 3FFF

Encoding: bits 63-60; bits 59-56; bits 55-16; bits 15-0

          Opcode        SR         Unused      Memory address

          0101

### PUTC
Prints the ASCII value of the contents of register 0 as a character to the console.

Example: PUTC

Encoding: bits 63-60; bits 59-0

          Opcode       Unused

          0110 

### PUTD
Prints the decimal value of register 0's contents to the console.

Example: PUTD

Encoding: bits 63-60; bits 59-0

          Opcode       Unused

          0111 

### HALT
Ends the program. All programs need this at the end of the file.

Example: HALT

Encoding: bits 63-60; bits 59-0

          Opcode       Unused

          1000