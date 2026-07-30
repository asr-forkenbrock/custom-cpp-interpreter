# Custom Programming Language - Inspired by Python

A lightweight interpreted programming language written entirely in C++. The project includes a hand-written lexer, recursive-descent parser, abstract syntax tree (AST), and tree-walking interpreter. The language uses Python-inspired syntax while remaining intentionally simple for educational purposes.

## Features
- Hand-written lexer
- Hand-written Recursive-descent parser
- Hand-written Abstract Syntax Tree (AST)
- Hand-written Tree-walking interpreter
- Variable assignment
- Integer, boolean, and string data types
- Arithmetic expressions
- Comparison operators
- Logical operators (`and`, `or`)
- `if` / `else` statements
- `while` loops
- `break` and `continue`
- User-defined functions
- Function return values
- Nested function calls
- Local function scopes
- Runtime error reporting
- Built-in standard library functions
- `//` comments 

### Built-in Functions

#### Math
- `sqrt(number)`
- `abs(number)`
- `min(a, b)`
- `max(a, b)`
- `pow(base, exponent)`
- `rand(min, max)`

#### String
- `len(string)`
- `upper(string)`
- `lower(string)`
- `substr(string, start, length)`
- `find(string, search)`

#### Type Conversion
- `to_string(number)`
- `to_number(string)`

#### Input
- `num_input()`
- `str_input()`

## Project Structure
```
Lexer.cpp          Tokenizes source code
Parser.cpp         Builds the Abstract Syntax Tree
Interpreter.cpp    Executes the AST
main.cpp           Program entry point
```

## Prerequisites
- C++17 compatible compiler
- g++

## Dependencies
- iostream
- string
- vector
- fstream
- unordered_map
- stack
- cmath
- random
- algorithm
- cctype

## Compilation

```bash
g++ main.cpp Lexer.cpp Parser.cpp Interpreter.cpp -std=c++17 -O2 -o asr-envi
```

Run:

```bash
./asr-envi
```

## Runtime Errors
The interpreter reports runtime errors including:
- Undefined variables
- Undefined functions
- Divide by zero
- Invalid operand types
- Invalid comparison types
- Invalid logical operations

## Known Limitations
- Function arguments cannot currently be expressions (only variables and literal values).
- Only integer numeric types are supported.
- No arrays or lists.
- No floating-point numbers.
- No classes or objects.
- No file I/O.
- No exception handling.
- Variables use function-level scope.
- Recursive function depth is limited by the system stack.

## Future Improvements
- Arrays and dictionaries
- Floating-point support
- `for` loops
- Additional standard library functions
- File I/O
- Import/module system
- Better runtime diagnostics
- Optimized AST execution