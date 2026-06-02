@echo off
echo ================================================
echo Testing Both Modes of DFA-NFA Converter
echo ================================================
echo.

echo [TEST 1] Mode 1: DFA to NFA (Example 1)
echo ================================================
(echo 1 & echo 1) | dfa_converter.exe
echo.
echo.

echo [TEST 2] Mode 2: NFA to DFA (Example 1)
echo ================================================
(echo 2 & echo 1) | dfa_converter.exe
echo.
echo.

echo [TEST 3] Mode 2: NFA to DFA (Example 4 - Complex)
echo ================================================
(echo 2 & echo 4) | dfa_converter.exe
echo.
echo.

echo ================================================
echo All Tests Complete!
echo ================================================
