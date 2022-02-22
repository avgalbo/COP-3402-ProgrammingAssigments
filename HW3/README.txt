## Homework 3 (Parser - Code Generator)

### This program was written by:
* Anthony Galbo
* Connor Cabrera
* Luis Curtiellas
* Eduardo Bourget

### How to use program
To compile enter `gcc driver.c lex.o parser.c vm.o -lm`. If the proper Makefile is present, simply enter `make`.

To execute after compiling enter `./a.out <input.txt>`, where `input.txt` contains the code to be parsed. You can append any combination of the following commands to print additional information:

* `-l` : print the list and table of lexemes/tokens to the screen
* `-s` : print the symbol table
* `-a` : print the generated assembly code (parser/codegen output) to the screen
* `-v` : print virtual machine execution trace to the screen

E.g. `./a.out input.txt –l –s –a -v` Print all types of output to the console.