@echo off
REM DFA-NFA Bidirectional Converter Build Script
REM Uses MSYS2 tools directly

setlocal

set MSYS2_PATH=C:\msys64\usr\bin
set PATH=%MSYS2_PATH%;%PATH%

echo ============================================
echo Building DFA-NFA Bidirectional Converter
echo ============================================
echo.

REM Create object directory
if not exist obj mkdir obj

REM Generate lexer from flex
echo [1/8] Generating lexer...
flex.exe -o obj\lex.yy.c src\lexer.l
if %errorlevel% neq 0 (
    echo ERROR: flex failed
    exit /b 1
)

REM Generate parser from bison
echo [2/8] Generating parser...
bison.exe -d -o obj\parser.tab.c src\parser.y
if %errorlevel% neq 0 (
    echo ERROR: bison failed
    exit /b 1
)

REM Compile DFA
echo [3/8] Compiling DFA module...
gcc.exe -Wall -std=c99 -I src -I obj -c -o obj\dfa.o src\dfa.c
if %errorlevel% neq 0 (
    echo ERROR: dfa.c compilation failed
    exit /b 1
)

REM Compile NFA
echo [4/8] Compiling NFA module...
gcc.exe -Wall -std=c99 -I src -I obj -c -o obj\nfa.o src\nfa.c
if %errorlevel% neq 0 (
    echo ERROR: nfa.c compilation failed
    exit /b 1
)

REM Compile Converter
echo [5/8] Compiling converter module...
gcc.exe -Wall -std=c99 -I src -I obj -c -o obj\converter.o src\converter.c
if %errorlevel% neq 0 (
    echo ERROR: converter.c compilation failed
    exit /b 1
)

REM Compile Main
echo [6/8] Compiling main program...
gcc.exe -Wall -std=c99 -I src -I obj -c -o obj\main.o src\main.c
if %errorlevel% neq 0 (
    echo ERROR: main.c compilation failed
    exit /b 1
)

REM Compile Lexer
echo [7/8] Compiling lexer...
gcc.exe -Wall -std=c99 -I src -I obj -c -o obj\lex.yy.o obj\lex.yy.c
if %errorlevel% neq 0 (
    echo ERROR: lex.yy.c compilation failed
    exit /b 1
)

REM Compile Parser
echo [8/8] Compiling parser...
gcc.exe -Wall -std=c99 -I src -I obj -c -o obj\parser.tab.o obj\parser.tab.c
if %errorlevel% neq 0 (
    echo ERROR: parser.tab.c compilation failed
    exit /b 1
)

REM Link all objects
echo.
echo Linking executable...
gcc.exe -o dfa_converter.exe obj\dfa.o obj\nfa.o obj\converter.o obj\lex.yy.o obj\parser.tab.o obj\main.o
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
echo Features:
echo   [1] DFA to NFA conversion
echo   [2] NFA to DFA conversion (Subset Construction)
echo.
echo To run:
echo   dfa_converter.exe
echo.

endlocal
