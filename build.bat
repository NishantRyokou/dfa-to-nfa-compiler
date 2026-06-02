@echo off
REM DFA to NFA Compiler - Manual Build Script
REM This script compiles without make, flex, or bison

echo Checking for flex and bison...
where flex >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: flex not found. Install MinGW/MSYS2
    echo Download: https://www.msys2.org/
    exit /b 1
)

where bison >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: bison not found. Install MinGW/MSYS2
    exit /b 1
)

where gcc >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: gcc not found. Install MinGW
    exit /b 1
)

echo Building DFA to NFA Converter...
echo.

REM Create object directory
if not exist obj mkdir obj

REM Generate lexer from flex
echo [1/5] Generating lexer...
flex -o obj\lex.yy.c src\lexer.l
if %errorlevel% neq 0 (
    echo ERROR: flex failed
    exit /b 1
)

REM Generate parser from bison
echo [2/5] Generating parser...
bison -d -o obj\parser.tab.c src\parser.y
if %errorlevel% neq 0 (
    echo ERROR: bison failed
    exit /b 1
)

REM Compile DFA
echo [3/5] Compiling DFA module...
gcc -Wall -std=c99 -I src -I obj -c -o obj\dfa.o src\dfa.c
if %errorlevel% neq 0 (
    echo ERROR: dfa.c compilation failed
    exit /b 1
)

REM Compile NFA
echo [4/5] Compiling NFA module...
gcc -Wall -std=c99 -I src -I obj -c -o obj\nfa.o src\nfa.c
if %errorlevel% neq 0 (
    echo ERROR: nfa.c compilation failed
    exit /b 1
)

REM Compile Converter
echo [5/5] Compiling converter module...
gcc -Wall -std=c99 -I src -I obj -c -o obj\converter.o src\converter.c
if %errorlevel% neq 0 (
    echo ERROR: converter.c compilation failed
    exit /b 1
)

REM Compile Main
echo [6/6] Compiling main program...
gcc -Wall -std=c99 -I src -I obj -c -o obj\main.o src\main.c
if %errorlevel% neq 0 (
    echo ERROR: main.c compilation failed
    exit /b 1
)

REM Compile Lexer
echo [7/7] Compiling lexer...
gcc -Wall -std=c99 -I src -I obj -c -o obj\lex.yy.o obj\lex.yy.c
if %errorlevel% neq 0 (
    echo ERROR: lex.yy.c compilation failed
    exit /b 1
)

REM Compile Parser
echo [8/8] Compiling parser...
gcc -Wall -std=c99 -I src -I obj -c -o obj\parser.tab.o obj\parser.tab.c
if %errorlevel% neq 0 (
    echo ERROR: parser.tab.c compilation failed
    exit /b 1
)

REM Link all objects
echo.
echo Linking executable...
gcc -o dfa_converter obj\dfa.o obj\nfa.o obj\converter.o obj\lex.yy.o obj\parser.tab.o obj\main.o
if %errorlevel% neq 0 (
    echo ERROR: linking failed
    exit /b 1
)

echo.
echo ============================================
echo BUILD SUCCESSFUL!
echo ============================================
echo Executable: dfa_converter.exe
echo.
echo To run:
echo   dfa_converter.exe ^< tests\test_input.dfa
echo.
