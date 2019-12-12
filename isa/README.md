## Files
| Name      | Description                                          |
|-----------|------------------------------------------------------|
| isa_cat.c | A program to print binarys in an assembly like form. |
| simulator.c | ISA simulator. Run `./simulator -h` for more info |


## The Instruction Set
The insruction set is based on [brainfuck](https://esolangs.org/wiki/Brainfuck).
Each cell is 32-bit. The 30-bit immediate is sign extended to 32-bit.
Byte addressed. Big endian. It is assumed that the program counter and the data
pointer are initialized to zero.

instruction format:<br>
```
Opcode  Imm 30-bit
00      000000000000000000000000000000
```

| Opcode |  Name       |  Description                                                                      |
|--------|-------------|-----------------------------------------------------------------------------------|
| 00     |  right imm  |  Move the data pointer to the right by imm.                                       |
| 01     |  add   imm  |  Add imm to the current address pointed to by the data pointer.                   |
| 10     |  bnz   imm  |  Set the program counter to program counter + imm if the current address pointed to by the data pointer is equal to zero. |

Any I/O is memory mapped.
