# nqoisc-cc (Not Quite One Instruction Set Computer C Compiler)
Note: Currently the compiler is still in development. It is currently not in a
usable state.

A small c compiler designed to target the NQOISC instruction set (an instruction
set with only three instructions).

### Design Goals
- Easily portable to new target architectures.
- Fast compilation time.
- Optimizing compiler.
- Aims to implement the C99 standard.

## Compiling
To compile nqoisc-cc you will need the following software installed:
- The [lemon](https://www.hwaci.com/sw/lemon/) parser generator.
- flex - lexer generator.
- gcc.

Run `build.sh` to build nqoisc-cc from source.

Currently, nqoisc-cc does not have a makefile but at some point, I will write one.

## The Instruction Set
The insruction set is based on [brainfuck](https://esolangs.org/wiki/Brainfuck).
Each cell is 32-bit. The 30-bit immediate is sign extended to 32-bit.
Byte addressed. Big endian. The program counter and the data pointer are
initialized to zero.

instruction format:<br>
```
Opcode  Imm 30-bit
00      000000000000000000000000000000
```

| Opcode |  Name       |  Description                                                    |
|--------|-------------|-----------------------------------------------------------------|
| 00     |  right imm  |  Move the data pointer to the right by imm.                     |
| 01     |  add   imm  |  Add imm to the current address pointed to by the data pointer. |
| 10     |  bnz   imm  |  Set the program counter to program counter + imm if the current address pointed to by the data pointer is equal to zero. |

Any I/O is memory mapped.

`nqoisc_toolchain` contains all files relating to the instruction set architecture.

## The Compiler Architecture
```
_________    __________    ______________    ____________    ________________
| Flex  |--->| Lemon  |--->| AST        |--->| Semantic |--->| AST          |___
| Lexer |    | Parser |    | Generation |    | Analysis |    | Optimization |  |
---------    ----------    --------------    ------------    ----------------  |
                                                             _______________   |
                                                             | Target Code |<---
                                                             | Generation  |
                                                             ---------------
```
