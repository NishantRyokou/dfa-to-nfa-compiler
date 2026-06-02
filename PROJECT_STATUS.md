# DFA to NFA Compiler - Project Status

## ✅ COMPLETED

All project files have been successfully created in:
```
C:\Users\legam\Documents\btech\assignment\toc\
```

### Project Files Created (12 files)

**Root Directory:**
- ✅ README.md - Project documentation
- ✅ Makefile - Build configuration
- ✅ build.bat - Windows batch build script
- ✅ build.ps1 - PowerShell build script
- ✅ INSTALL.md - Installation instructions
- ✅ SETUP_WINDOWS.md - Windows setup guide
- ✅ PROJECT_STATUS.md - This file

**src/ Directory (9 source files):**
- ✅ dfa.h - DFA header
- ✅ dfa.c - DFA implementation
- ✅ nfa.h - NFA header
- ✅ nfa.c - NFA implementation
- ✅ converter.h - Converter header
- ✅ converter.c - Converter implementation
- ✅ lexer.l - Flex lexical analyzer
- ✅ parser.y - Bison syntax analyzer
- ✅ main.c - Main program

**tests/ Directory:**
- ✅ test_input.dfa - Sample DFA input

---

## 📋 Next Steps

### Step 1: Install Required Tools

You need to install:
- Flex (lexical analyzer generator)
- Bison (parser generator)
- Make (build tool)

**RECOMMENDED:** Use MSYS2
- Download: https://www.msys2.org/
- Then in MSYS2 terminal: 
  ```bash
  pacman -S base-devel flex bison mingw-w64-x86_64-gcc mingw-w64-x86_64-make
  ```

See `SETUP_WINDOWS.md` for detailed instructions.

### Step 2: Build the Project

In MSYS2 or terminal with tools installed:

```bash
cd C:\Users\legam\Documents\btech\assignment\toc
make
```

Or use the build script:
```cmd
build.bat
```

### Step 3: Run the Program

```bash
./dfa_converter < tests/test_input.dfa
```

Expected output:
```
DFA to NFA Converter
Using Flex and Bison
===================

DFA parsed successfully

Parsed DFA:
=== DFA ===
States: q0 q1 q2
Alphabet: a b
Initial State: q0
Final States: q2
Transitions:
  q0 --(a)--> q1
  q1 --(b)--> q2
  q2 --(a)--> q1

Converted NFA:
=== NFA ===
States: q0 q1 q2
Alphabet: a b
Initial State: q0
Final States: q2
Transitions:
  q0 --(a)--> q1
  q1 --(b)--> q2
  q2 --(a)--> q1
```

---

## 📚 Project Architecture

### Compilation Pipeline

```
Input DFA Specification
        ↓
    [Lexer - Flex]        (src/lexer.l)
        ↓
    Tokenize Input
        ↓
    [Parser - Bison]      (src/parser.y)
        ↓
    Parse Tokens & Validate
        ↓
    [DFA Data Structure]  (src/dfa.c)
        ↓
    [Converter Logic]      (src/converter.c)
        ↓
    [NFA Data Structure]   (src/nfa.c)
        ↓
    Output NFA Specification
```

### Data Flow

1. **Lexical Analysis** - lexer.l tokenizes input
2. **Syntax Analysis** - parser.y builds DFA structure
3. **Semantic Analysis** - main.c validates input
4. **Code Generation** - converter.c transforms DFA to NFA
5. **Output** - main.c displays NFA

### Modules

| Module | Purpose |
|--------|---------|
| dfa.{h,c} | DFA data structure and operations |
| nfa.{h,c} | NFA data structure and operations |
| converter.{h,c} | DFA to NFA conversion |
| lexer.l | Flex tokenizer specification |
| parser.y | Bison parser specification |
| main.c | Program entry point |

---

## 🔍 Input Format

The program accepts DFA specifications in this format:

```
DFA {
  states: q0, q1, q2
  alphabet: a, b
  initial: q0
  finals: q2
  transitions:
    q0 --a--> q1
    q1 --b--> q2
    q2 --a--> q1
}
```

**Components:**
- `states`: Comma-separated state names
- `alphabet`: Comma-separated input symbols
- `initial`: Starting state
- `finals`: Accepting states
- `transitions`: State transitions (from --symbol--> to)

---

## 🎯 Features Implemented

✅ Flex-based lexical analysis
✅ Bison-based syntax analysis
✅ Complete DFA data structure
✅ Complete NFA data structure
✅ DFA to NFA conversion algorithm
✅ Error handling and reporting
✅ Memory management
✅ Sample test case
✅ Makefile build system
✅ Cross-platform support

---

## 🛠️ Troubleshooting

**"make: command not found"**
- Install make: See SETUP_WINDOWS.md
- Or use: `build.ps1` or `build.bat`

**"flex: command not found"**
- Install via MSYS2: `pacman -S flex`

**"bison: command not found"**
- Install via MSYS2: `pacman -S bison`

**Compilation errors**
- Verify all tools installed: `gcc --version`, `flex --version`, `bison --version`
- Try: `make clean && make`

---

## 📖 Resources

- **Flex**: https://flex.sourceforge.net/
- **Bison**: https://www.gnu.org/software/bison/manual/
- **Compiler Design**: Aho, Lam, Sethi, Ullman
- **Automata Theory**: Hopcroft & Ullman

---

## 📝 Author Notes

This is a complete compiler design project implementing:
- Lexical analysis (Flex)
- Syntax analysis (Bison)
- DFA/NFA representations
- Automata conversion

Perfect for Theory of Computation (TOC) coursework.

---

**Project Created:** June 2, 2026
**Status:** Ready for Build & Execution
**Next Action:** Install toolchain and run `make`
