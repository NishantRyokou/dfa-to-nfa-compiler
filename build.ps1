# DFA to NFA Compiler Build Script (PowerShell)
# This script attempts to build using available tools

param(
    [switch]$Clean
)

$ErrorActionPreference = "Stop"

$basePath = Split-Path -Parent $MyInvocation.MyCommand.Path
$srcDir = Join-Path $basePath "src"
$objDir = Join-Path $basePath "obj"
$testDir = Join-Path $basePath "tests"

# Colors
$Green = [System.ConsoleColor]::Green
$Red = [System.ConsoleColor]::Red
$Yellow = [System.ConsoleColor]::Yellow

function Write-Status {
    param([string]$Message)
    Write-Host $Message -ForegroundColor $Green
}

function Write-Error {
    param([string]$Message)
    Write-Host "ERROR: $Message" -ForegroundColor $Red
    exit 1
}

function Write-Warning {
    param([string]$Message)
    Write-Host "WARNING: $Message" -ForegroundColor $Yellow
}

# Check for required tools
Write-Host "Checking for required tools..." -ForegroundColor Cyan

$haveGcc = $null -ne (Get-Command gcc -ErrorAction SilentlyContinue)
$haveFlex = $null -ne (Get-Command flex -ErrorAction SilentlyContinue)
$haveBison = $null -ne (Get-Command bison -ErrorAction SilentlyContinue)

Write-Host "  gcc: $(if ($haveGcc) { 'Found' } else { 'NOT FOUND' })"
Write-Host "  flex: $(if ($haveFlex) { 'Found' } else { 'NOT FOUND' })"
Write-Host "  bison: $(if ($haveBison) { 'Found' } else { 'NOT FOUND' })"

if (-not $haveGcc) {
    Write-Error "gcc not found. Install MinGW-w64"
}

if (-not $haveFlex) {
    Write-Warning "flex not found. Install from MSYS2 or GnuWin32"
    Write-Host "See INSTALL.md for instructions"
    exit 1
}

if (-not $haveBison) {
    Write-Warning "bison not found. Install from MSYS2"
    Write-Host "See INSTALL.md for instructions"
    exit 1
}

# Clean if requested
if ($Clean) {
    Write-Status "Cleaning build artifacts..."
    Remove-Item -Path $objDir -Recurse -Force -ErrorAction SilentlyContinue
    Remove-Item -Path (Join-Path $basePath "dfa_converter.exe") -Force -ErrorAction SilentlyContinue
    Remove-Item -Path (Join-Path $basePath "dfa_converter") -Force -ErrorAction SilentlyContinue
    Write-Status "Clean complete"
    exit 0
}

# Create obj directory
if (-not (Test-Path $objDir)) {
    New-Item -ItemType Directory -Path $objDir | Out-Null
}

Write-Status "Building DFA to NFA Converter..."
Write-Host ""

# Generate lexer
Write-Host "[1/8] Generating lexer with flex..." -ForegroundColor Cyan
& flex -o "$objDir\lex.yy.c" "$srcDir\lexer.l"
if ($LASTEXITCODE -ne 0) { Write-Error "flex failed" }

# Generate parser
Write-Host "[2/8] Generating parser with bison..." -ForegroundColor Cyan
& bison -d -o "$objDir\parser.tab.c" "$srcDir\parser.y"
if ($LASTEXITCODE -ne 0) { Write-Error "bison failed" }

# Compile modules
$compileFiles = @(
    @{file = "src/dfa.c"; obj = "dfa.o"; step = 3}
    @{file = "src/nfa.c"; obj = "nfa.o"; step = 4}
    @{file = "src/converter.c"; obj = "converter.o"; step = 5}
    @{file = "src/main.c"; obj = "main.o"; step = 6}
)

foreach ($item in $compileFiles) {
    Write-Host "[$($item.step)/8] Compiling $($item.file)..." -ForegroundColor Cyan
    $sourceFile = Join-Path $srcDir ([System.IO.Path]::GetFileName($item.file))
    $objFile = Join-Path $objDir $item.obj
    & gcc -Wall -std=c99 -I $srcDir -I $objDir -c -o $objFile $sourceFile
    if ($LASTEXITCODE -ne 0) { Write-Error "Compilation of $($item.file) failed" }
}

# Compile generated files
Write-Host "[7/8] Compiling generated lexer..." -ForegroundColor Cyan
& gcc -Wall -std=c99 -I $srcDir -I $objDir -c -o "$objDir\lex.yy.o" "$objDir\lex.yy.c"
if ($LASTEXITCODE -ne 0) { Write-Error "lex.yy.c compilation failed" }

Write-Host "[8/8] Compiling generated parser..." -ForegroundColor Cyan
& gcc -Wall -std=c99 -I $srcDir -I $objDir -c -o "$objDir\parser.tab.o" "$objDir\parser.tab.c"
if ($LASTEXITCODE -ne 0) { Write-Error "parser.tab.c compilation failed" }

# Link
Write-Host ""
Write-Host "Linking executable..." -ForegroundColor Cyan
$objFiles = @(
    "$objDir\dfa.o"
    "$objDir\nfa.o"
    "$objDir\converter.o"
    "$objDir\lex.yy.o"
    "$objDir\parser.tab.o"
    "$objDir\main.o"
)

$exePath = Join-Path $basePath "dfa_converter"
& gcc -o $exePath @objFiles
if ($LASTEXITCODE -ne 0) { Write-Error "Linking failed" }

Write-Host ""
Write-Host "============================================" -ForegroundColor Green
Write-Status "BUILD SUCCESSFUL!"
Write-Host "============================================" -ForegroundColor Green
Write-Host ""
Write-Host "Executable: $exePath"
Write-Host ""
Write-Host "To run the program:"
Write-Host "  cmd /c `"$exePath < tests\test_input.dfa`""
Write-Host ""
