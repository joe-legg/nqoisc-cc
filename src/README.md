# The Compiler Source Code
## Source Files
| File              | Description                                                                                 |
|-------------------|---------------------------------------------------------------------------------------------|
| `ast.h`           | Declarations for the AST (**A**bstract **S**yntax **T**ree).                                |
| `ast.c`           | Implementation for the AST.                                                                 |
| `error.h`         | Declarations for `error.c`.                                                                 |
| `error.c`         | Error printing and handling.                                                                |
| `hash_table.h`    | Declarations for hash table.                                                                |
| `hash_table.c`    | Hash table implementation.                                                                  |
| `lexer.l`         | FLEX based lexer/tokenizer.                                                                 |
| `main.c`          | Main compiler entry point.                                                                  |
| `malloc_or_die.h` | Declarations for `malloc_or_die.h`.                                                         |
| `malloc_or_die.c` | A simple function that trys to allocate memory, if it fails it exits the compiler.          |
| `parser.lemon`    | LEMON based parser for C.                                                                   |
| `vector.h`        | Declarations for `vector.c`.                                                                |
| `vector.c`        | Simple dynamic vector implementation.                                                       |
