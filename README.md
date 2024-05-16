# asm-vm

## About
This virtual machine reads in an assembly program, converts the assembly instructions into machine code, stores the instructions in virtual memory, then executes the instructions sequentially. This program is essentially a virtual machine and assembler wrapped up into just over 300 lines of C code. The VM runs programs that consist of a 64-bit assembly language that I've made, which is based on the LC-3 instruction set. The key difference between the two languages is the number of instructions and the instruction length. So far I've only implemented 9 instructions (more on the way), 3 of which are trap routines to deal with I/O, and all the encoding of the instructions is done in 64 bits, and all the general purpose registers are also 64 bits wide. I went with 64-bit instructions instead of the 16-bits that the LC-3 uses, because I wanted to be able to generate machine code that contained the instruction's opcode, register numbers, flags, and still have space for a standard 32-bit integer that could be loaded into a register or used some other way.

For more details on this assembly language, including usage examples and instruction encoding, reference ```instruction-set.md``` located in the ```docs``` directory. I've also included a copy of the LC-3 instruction set if anyone is interested in the similarities and differences between them.

I also have to give credit to a [blog post](https://www.jmeiners.com/lc3-vm/) by Justin Meiners that explains how to write a virtual machine in C or C++. It helped me understand how to structure this type of program, how the registers and memory should be set up, and how to parse and execute machine code within a VM. Another reference I used was the textbook [Introduction to Computing Systems, 2nd Edition, by Yale Patt and Sanjay Patel](https://github.com/Tedko/ece/blob/master/198jl/hwAndExtras/Yale%20Patt%2C%20Sanjay%20Patel-Introduction%20to%20Computing%20Systems_%20From%20bits%20and%20gates%20to%20C%20and%20beyond-McGraw-Hill%20(2005).pdf), which helped me get a better understanding of computer architecture and how assembly languages generally function.

## Get Started
You will need to have Git and GCC installed to download, compile, and run the program. You can use a different C compiler, but you will need to update the Makefile's CC parameter. I've only tested this out on Linux so far, but it should work fine on Windows and MacOS as long as you have a C compiler.

Download the files to your machine.
```bash
git clone https://github.com/aidan-palmer/asm-vm.git
```
Navigate into ```asm-vm/src``` and compile the program with ```make```. You can also run ```make hello``` to try out a very efficient and not very readable hello world program written in my assembly language. You can find its source code in the ```tests``` directory.

To run your own assembly program, compile the program and then pass the name of your ```.s``` or ```.asm``` file as an argument:
```bash
./vm program.s
```