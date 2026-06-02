# Installation & Build Instructions

## Prerequisites

You need to install:
- **Flex** - Lexical analyzer generator
- **Bison** - Parser generator
- **Make** - Build tool

## Installation on Windows

### Option 1: MSYS2 (Recommended)

1. Download MSYS2 from https://www.msys2.org/
2. Install to `C:\msys64`
3. Open MSYS2 Terminal (MSYS2 MinGW 64-bit)
4. Install tools:
   ```bash
   pacman -S flex bison make mingw-w64-x86_64-gcc
   ```

5. Navigate to your project:
   ```bash
   cd /c/Users/legam/Documents/btech/assignment/toc
   ```

6. Build:
   ```bash
   make
   ```

7. Run:
   ```bash
   ./dfa_converter < tests/test_input.dfa
   ```

### Option 2: MinGW-w64 with make

If you already have MinGW-w64:

1. Install make:
   - Download from: https://gnuwin32.sourceforge.net/packages/make.htm
   - Or use Chocolatey: `choco install make`

2. In Command Prompt:
   ```cmd
   cd C:\Users\legam\Documents\btech\assignment\toc
   make
   dfa_converter.exe < tests\test_input.dfa
   ```

## Manual Build (if make not available)

Run the build script:
```cmd
cd C:\Users\legam\Documents\btech\assignment\toc
build.bat
```

Then run:
```cmd
dfa_converter.exe < tests\test_input.dfa
```

## Verify Installation

From Command Prompt or PowerShell (with MSYS2):
```bash
gcc --version
flex --version
bison --version
make --version
```

All should show version information.
