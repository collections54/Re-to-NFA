Re to NFA project

List of files:
main.c - Handles reading command line arguments and opening files
stack.h - Header file for stack functions
stack.c - Contains code for stack functions
nfa.h - Header file for nfa.c and evaluate.c
nfa.c - Contains code for creating nfas
evaluate.c - contains code for creating an nfa from a RE
makefile - makefile for compiling code

Compiler version: gcc (Ubuntu 13.2.0-23ubuntu4) 13.2.0
To Compile:
Run "make" command after extracting files.
make -clean to remove -o files and executable

To Run:
run command  "./re_to_nfa file1" where file1 is the file containing RE strings 
A minimum of one file is required to run.
Multiple files can be run at once by adding filename to command line
Output is printed to stdout, error messages printed to stderr 
