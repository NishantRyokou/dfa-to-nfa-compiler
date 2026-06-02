#!/bin/bash

# Verification script for optimization changes
# Run this BEFORE building to verify all changes are in place

echo "╔════════════════════════════════════════════╗"
echo "║   Optimization Verification Script         ║"
echo "╚════════════════════════════════════════════╝"
echo ""

ERRORS=0

# Check dfa.h for dynamic allocation
echo "[1/10] Checking dfa.h for dynamic structures..."
if grep -q "State\* states;" src/dfa.h && grep -q "int capacity_states;" src/dfa.h; then
    echo "✓ dfa.h has dynamic allocation"
else
    echo "✗ dfa.h missing dynamic allocation"
    ERRORS=$((ERRORS+1))
fi

# Check dfa.c for string interning
echo "[2/10] Checking dfa.c for string interning..."
if grep -q "intern_string" src/dfa.c; then
    echo "✓ dfa.c has string interning"
else
    echo "✗ dfa.c missing string interning"
    ERRORS=$((ERRORS+1))
fi

# Check dfa.c for growth factor
echo "[3/10] Checking dfa.c for growth factor..."
if grep -q "GROWTH_FACTOR" src/dfa.c; then
    echo "✓ dfa.c has growth factor defined"
else
    echo "✗ dfa.c missing growth factor"
    ERRORS=$((ERRORS+1))
fi

# Check nfa.h for dynamic allocation
echo "[4/10] Checking nfa.h for dynamic structures..."
if grep -q "State\* states;" src/nfa.h && grep -q "int capacity_states;" src/nfa.h; then
    echo "✓ nfa.h has dynamic allocation"
else
    echo "✗ nfa.h missing dynamic allocation"
    ERRORS=$((ERRORS+1))
fi

# Check nfa.c for string interning
echo "[5/10] Checking nfa.c for string interning..."
if grep -q "intern_string" src/nfa.c; then
    echo "✓ nfa.c has string interning"
else
    echo "✗ nfa.c missing string interning"
    ERRORS=$((ERRORS+1))
fi

# Check nfa.c for proper cleanup
echo "[6/10] Checking nfa.c for proper cleanup..."
if grep -q "free(nfa->transitions\[i\].to_states)" src/nfa.c; then
    echo "✓ nfa.c has proper cleanup"
else
    echo "✗ nfa.c missing cleanup code"
    ERRORS=$((ERRORS+1))
fi

# Check main.c for buffered I/O
echo "[7/10] Checking main.c for buffered I/O..."
if grep -q "FILE_BUFFER_SIZE" src/main.c && grep -q "setvbuf" src/main.c; then
    echo "✓ main.c has buffered I/O"
else
    echo "✗ main.c missing buffered I/O"
    ERRORS=$((ERRORS+1))
fi

# Check main.c for cleanup function
echo "[8/10] Checking main.c for cleanup function..."
if grep -q "cleanup_examples" src/main.c; then
    echo "✓ main.c has cleanup function"
else
    echo "✗ main.c missing cleanup function"
    ERRORS=$((ERRORS+1))
fi

# Check main.c for dynamic example array
echo "[9/10] Checking main.c for dynamic example array..."
if grep -q "DFAExample\* examples" src/main.c; then
    echo "✓ main.c uses dynamic example array"
else
    echo "✗ main.c missing dynamic example array"
    ERRORS=$((ERRORS+1))
fi

# Check dfa.h and nfa.h consistency
echo "[10/10] Checking dfa.h and nfa.h for consistency..."
if grep -q "char\* string_pool" src/dfa.h && grep -q "char\* string_pool" src/nfa.h; then
    echo "✓ Both modules have string pool"
else
    echo "✗ Inconsistent string pool implementation"
    ERRORS=$((ERRORS+1))
fi

echo ""
echo "════════════════════════════════════════════"

if [ $ERRORS -eq 0 ]; then
    echo "✓ ALL CHECKS PASSED!"
    echo "Ready to build with: make clean && make"
    exit 0
else
    echo "✗ VERIFICATION FAILED ($ERRORS issues found)"
    echo "Fix the issues before building"
    exit 1
fi

