#!/bin/bash
gcc -Wall -g -o bin/simulator simulator.c
gcc -Wall -g -o bin/isa-cat isa_cat.c
gcc -Wall -g -o bin/asm asm.c ../src/vector.c
