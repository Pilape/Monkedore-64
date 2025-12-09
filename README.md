# Monkedore 64

A stack based virtual machine/fantasy console.

## How to run
The program is meant to be embeddable in other programs.
You can try out the vm by executing the run executable. You may have to build the execute_program.c yourself. It only uses the standard library.
Example command:
```Bash
gcc -o run execute_program.c
```
The executable takes the path to a .rom file as an argument. You can also add a second argument of any kind to walk through the program step by step.
To step forward in this mode you need to input an integer of any value through the command line.

None of my example programs work as intended because I suck at writing in my own programming language :(
