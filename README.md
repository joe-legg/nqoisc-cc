# jcc (Joes C Compiler)
Note: Currently the compiler is still in development. The parser is almost
finished. Once the parser has finished I will work on implementing a
prepocessor.

A small c compiler designed to target an instruction set with only three
instructions.

### Design Goals
- Easily portable to new target architectures.
- Fast compilation time.
- Optimizing compiler.
- Aims to implement the C99 standard.

## Compiling
Run ```build.sh``` to build jcc from source.

## The Instruction Set
The insruction set is based on [brainfuck](https://esolangs.org/wiki/Brainfuck).
Each cell is 32-bit. The 30-bit immediate is sign extended to 32-bit.

instruction format:<br>
```
Opcode  Imm 30-bit
00      000000000000000000000000000000
```

| Opcode |  Name       |  Description                                              |
|--------|-------------|-----------------------------------------------------------|
| 00     |  right imm  |  Move the cell pointer to the right by imm.               |
| 01     |  add   imm  |  Add imm to the cell under the pointer.                   |
| 10     |  bnz   imm  |  Set the instruction pointer to instruction pointer + imm if the current cell is equal to zero.                    |

Any I/O is memory mapped.

## The Compiler Architecture
```
_________    __________    ______________    ____________    ________________
| Flex  |--->| Lemon  |--->| AST        |--->| Semantic |--->| AST          |___
| Lexer |    | Parser |    | Generation |    | Analysis |    | Optimization |  |
---------    ----------    --------------    ------------    ----------------  |
                        _______________    ________________    ______________  |
                        | Target Code |<---| IR           |<---| IR         |<--
                        | Generation  |    | Optimization |    | Generation |
                        ---------------    ----------------    --------------
```
