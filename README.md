# DFA to NFA Converter - Compiler Design Project

## Overview

This project implements a complete compiler using **Flex** (lexical analyzer) and **Bison** (syntax analyzer) to parse DFA specifications and convert them to NFA.

## Features

✅ **Flex Lexical Analyzer** - Tokenizes DFA input
✅ **Bison Syntax Analyzer** - Parses and validates DFA syntax
✅ **10 Built-in DFA Examples** - Ready to use test cases
✅ **Interactive Menu** - User-friendly example selection
✅ **Complete Conversion** - DFA to NFA transformation
✅ **Detailed Output** - Shows both input DFA and output NFA

## Project Structure

```
toc/
├── src/                 # Source code
│   ├── dfa.h/c         # DFA data structures
│   ├── nfa.h/c         # NFA data structures
│   ├── converter.h/c   # Conversion logic
│   ├── lexer.l         # Flex lexical analyzer
│   ├── parser.y        # Bison syntax parser
│   ├── main.c          # Interactive main program
│   └── tokens.h        # Token definitions
├── tests/
│   └── dfa_examples.txt # 10 DFA examples
├── Makefile             # Build configuration
└── README.md
```

## Building

```bash
cd C:\Users\legam\Documents\btech\assignment\toc
make
```

## Running

```bash
./dfa_converter
```

The program will:
1. Load 10 DFA examples from `tests/dfa_examples.txt`
2. Display all examples with their specifications
3. Ask you to select which example to convert
4. Parse the selected DFA
5. Convert it to NFA
6. Display both DFA and NFA specifications

## Example Usage

```
========================================
                DFA to NFA Converter              
========================================

Successfully loaded 10 DFA examples.

========================================
Available DFA Examples:
========================================

[1] DFA { states: q0, q1, q2, q3 alphabet: a, b initial: q0 finals: q3 }

[2] DFA { states: s0, s1, s2, s3 alphabet: 0, 1 initial: s0 finals: s3 }

... (more examples)

Enter the number of the DFA example to convert (1-10): 1

========================================
Converting Example 1
========================================

DFA parsed successfully

>>> INPUT DFA <<<
=== DFA ===
States: q0 q1 q2 q3
Alphabet: a b
Initial State: q0
Final States: q3
Transitions:

>>> CONVERTED NFA <<<
=== NFA ===
States: q0 q1 q2 q3
Alphabet: a b
Initial State: q0
Final States: q3
Transitions:

========================================
Conversion Complete!
========================================
```

## DFA Input Format

Each DFA is specified in a single line with the following structure:

```
DFA { states: state1, state2, ... alphabet: sym1, sym2, ... initial: initial_state finals: final1, final2, ... }
```

### Components:
- **states**: Comma-separated state names
- **alphabet**: Comma-separated input symbols
- **initial**: Starting state (single state)
- **finals**: Comma-separated accepting states

## Compilation Stages

1. **Lexical Analysis (Flex)**
   - Tokenizes DFA specification
   - Recognizes keywords, identifiers, and symbols
   - Produces token stream

2. **Syntax Analysis (Bison)**
   - Parses token stream
   - Validates grammar
   - Builds DFA data structure

3. **Semantic Analysis**
   - Validates DFA correctness
   - Ensures all states and symbols are properly defined

4. **Code Generation**
   - Converts DFA to equivalent NFA
   - Displays both representations

## Built-in Examples

The program includes 10 diverse DFA examples:

1. **Example 1**: 4 states, 2-symbol alphabet (a, b)
2. **Example 2**: 4 states, 2-symbol alphabet (0, 1)
3. **Example 3**: 3 states, 2-symbol alphabet (x, y)
4. **Example 4**: 3 states, 1-symbol alphabet (a)
5. **Example 5**: 5 states, 3-symbol alphabet (0, 1, 2)
6. **Example 6**: 3 states, 3-symbol alphabet (c, d, e)
7. **Example 7**: 4 states, 2-symbol alphabet (i, j)
8. **Example 8**: 4 states, 3-symbol alphabet (u, v, w)
9. **Example 9**: 3 states, 1-symbol alphabet (k)
10. **Example 10**: 5 states, 2-symbol alphabet (l, m)

## Requirements

- GCC compiler
- Flex (lexical analyzer generator)
- Bison (parser generator)
- Make (build tool)

All installed via MSYS2:
```bash
pacman -S base-devel flex bison mingw-w64-x86_64-gcc mingw-w64-x86_64-make
```

## Troubleshooting

**"Error: Could not open file tests/dfa_examples.txt"**
- Make sure you're running from the correct directory
- Ensure `tests/dfa_examples.txt` exists

**"Invalid input. Please enter a number."**
- Enter a valid number between 1 and 10

**"Parsing failed"**
- The DFA specification has syntax errors
- Check the format of the selected example

## Project Features Implemented

✅ Complete Flex lexical analyzer
✅ Complete Bison syntax parser
✅ DFA and NFA data structures
✅ Interactive user menu
✅ Multiple DFA examples
✅ DFA to NFA conversion
✅ Error handling
✅ Memory management
✅ Clean output formatting

## Educational Value

This project demonstrates:
- Lexical analysis with Flex
- Syntax analysis with Bison
- Compiler design principles
- Automata theory (DFA/NFA conversion)
- C programming and data structures
- Interactive program design

## Author

Created as a Theory of Computation (TOC) / Compiler Design project.

---

**Status:** ✅ Complete and Fully Functional
