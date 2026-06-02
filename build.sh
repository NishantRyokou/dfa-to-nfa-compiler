#!/bin/bash
# DFA to NFA Compiler Build Script for MSYS2

set -e

echo "DFA to NFA Compiler - Build Script"
echo "==================================="
echo ""

# Navigate to project directory
cd "$(dirname "$0")"
PROJECT_DIR=$(pwd)

echo "Project directory: $PROJECT_DIR"
echo ""

# Check for required tools
echo "Checking for required tools..."
for tool in gcc flex bison make; do
    if command -v $tool &> /dev/null; then
        version=$($tool --version 2>&1 | head -n1)
        echo "✓ $tool: $version"
    else
        echo "✗ $tool: NOT FOUND"
        if [ "$tool" = "make" ] || [ "$tool" = "flex" ] || [ "$tool" = "bison" ]; then
            echo "  Installing $tool..."
            pacman -S --noconfirm $tool
        fi
    fi
done

echo ""
echo "Building..."
echo ""

# Create obj directory
mkdir -p obj

# Run make
make

echo ""
echo "============================================"
echo "BUILD SUCCESSFUL!"
echo "============================================"
echo ""
echo "To run the program:"
echo "  ./dfa_converter < tests/test_input.dfa"
echo ""
