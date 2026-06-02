# Building the Optimized DFA to NFA Compiler

## Quick Start (3 Steps)

1. **Open MSYS2 MinGW 64-bit** from your Start Menu
2. **Run these commands**:
   ```bash
   cd /c/Users/legam/Documents/btech/assignment/toc
   make clean && make
   ```
3. **Test**:
   ```bash
   ./dfa_converter
   ```

---

## Detailed Build Guide

### Step 1: Verify MSYS2 is Installed

Open PowerShell and check:
```powershell
C:\msys64\mingw64\bin\gcc --version
```

If this fails, install MSYS2 from: https://www.msys2.org/

### Step 2: Verify Build Tools

Open **MSYS2 MinGW 64-bit** terminal and check:

```bash
gcc --version          # Should output version info
flex --version         # Should output version info
bison --version        # Should output version info
make --version         # Should output version info
```

If any tool is missing, install with:
```bash
pacman -S base-devel flex bison mingw-w64-x86_64-gcc mingw-w64-x86_64-make
```

### Step 3: Navigate to Project

In the MSYS2 terminal:
```bash
cd /c/Users/legam/Documents/btech/assignment/toc
```

Verify you're in the right directory:
```bash
ls src/          # Should list: dfa.c dfa.h lexer.l main.c nfa.c nfa.h parser.y converter.c converter.h tokens.h
ls tests/        # Should list: dfa_examples.txt test_input.dfa
```

### Step 4: Clean Previous Build

```bash
make clean
```

Expected output:
```
rm -rf obj dfa_converter
```

### Step 5: Build Optimized Code

```bash
make
```

**Expected output:**
```
gcc -Wall -std=c99 -I src -I obj -c -o obj/dfa.o src/dfa.c
gcc -Wall -std=c99 -I src -I obj -c -o obj/nfa.o src/nfa.c
gcc -Wall -std=c99 -I src -I obj -c -o obj/converter.o src/converter.c
gcc -Wall -std=c99 -I src -I obj -c -o obj/main.o src/main.c
flex -o obj/lex.yy.c src/lexer.l
bison -d -o obj/parser.tab.c src/parser.y
gcc -Wall -std=c99 -I src -I obj -c -o obj/lex.yy.o obj/lex.yy.c
gcc -Wall -std=c99 -I src -I obj -c -o obj/parser.tab.o obj/parser.tab.c
gcc -o dfa_converter obj/dfa.o obj/nfa.o obj/converter.o obj/lex.yy.o obj/parser.tab.o obj/main.o
```

No errors should appear. If there are errors, see Troubleshooting below.

### Step 6: Verify Build Success

Check that the executable was created:
```bash
ls -lh dfa_converter
```

Should output something like:
```
-rwxr-xr-x 1 user group 45K Jun  2 12:34 dfa_converter*
```

The file should be executable and ~45 KB.

### Step 7: Test the Executable

```bash
./dfa_converter
```

You should see:
```
╔════════════════════════════════════════════╗
║      DFA to NFA Converter                  ║
║      Using Flex and Bison                  ║
╚════════════════════════════════════════════╝

Successfully loaded 10 DFA examples.

========================================
Available DFA Examples:
========================================

[1] states: q0, q1, q2 alphabet: a, b initial: q0 finals: q2 transitions: q0 --a--> q1, q1 --b--> q2, q2 --a--> q1

[2] states: s0, s1, s2, s3 alphabet: 0, 1 initial: s0 finals: s3 transitions: s0 --0--> s1, s1 --1--> s2, s2 --0--> s3, s3 --1--> s0

...and 8 more examples...

Enter the number of the DFA example to convert (1-10): 
```

Type `1` and press Enter:
```
Enter the number of the DFA example to convert (1-10): 1

========================================
Converting Example 1
========================================

>>> INPUT DFA <<<
=== DFA ===
States: q0 q1 q2
Alphabet: a b
Initial State: q0
Final States: q2
Transitions:
  q0 --(a)--> q1
  q1 --(b)--> q2
  q2 --(a)--> q1

>>> CONVERTED NFA <<<
=== NFA ===
States: q0 q1 q2
Alphabet: a b
Initial State: q0
Final States: q2
Transitions:
  q0 --(a)--> q1
  q1 --(b)--> q2
  q2 --(a)--> q1

========================================
Conversion Complete!
========================================
```

If you see this output, the build is **successful**! ✓

---

## Testing All Examples

Create a test script to verify all 10 examples work:

### On Linux/Mac/MSYS2 - Create `test_all.sh`:

```bash
#!/bin/bash

echo "Testing all 10 DFA examples..."
echo ""

for i in {1..10}; do
    echo -n "Testing example $i... "
    if echo $i | ./dfa_converter > /tmp/test_$i.log 2>&1; then
        if grep -q "Conversion Complete" /tmp/test_$i.log; then
            echo "✓ PASS"
        else
            echo "✗ FAIL (no completion message)"
        fi
    else
        echo "✗ FAIL (exit code error)"
    fi
done

echo ""
echo "All tests completed!"
```

Run it:
```bash
chmod +x test_all.sh
./test_all.sh
```

### On Windows PowerShell - Create `test_all.ps1`:

```powershell
Write-Host "Testing all 10 DFA examples..."
Write-Host ""

for ($i = 1; $i -le 10; $i++) {
    Write-Host -NoNewline "Testing example $i... "
    
    $output = $i | .\dfa_converter 2>&1
    
    if ($LASTEXITCODE -eq 0 -and $output -match "Conversion Complete") {
        Write-Host "✓ PASS" -ForegroundColor Green
    } else {
        Write-Host "✗ FAIL" -ForegroundColor Red
        Write-Host $output
    }
}

Write-Host ""
Write-Host "All tests completed!"
```

Run it:
```powershell
powershell -ExecutionPolicy Bypass -File test_all.ps1
```

---

## Troubleshooting

### Error: "gcc: command not found"

The compiler isn't in PATH. 

**Solution**: Run from MSYS2 MinGW 64-bit terminal, not regular PowerShell.

```bash
# Open MSYS2 MinGW 64-bit from Start Menu, then:
cd /c/Users/legam/Documents/btech/assignment/toc
make clean && make
```

### Error: "flex: command not found"

The lexer isn't installed.

**Solution**: In MSYS2 terminal:
```bash
pacman -S flex
```

Then try again:
```bash
make clean && make
```

### Error: "bison: command not found"

The parser isn't installed.

**Solution**: In MSYS2 terminal:
```bash
pacman -S bison
```

Then try again:
```bash
make clean && make
```

### Error: "make: command not found"

Make isn't installed.

**Solution**: In MSYS2 terminal:
```bash
pacman -S make mingw-w64-x86_64-make
```

Then try again:
```bash
make clean && make
```

### Error: "No such file or directory: src/dfa.c"

You're not in the right directory.

**Solution**: 
```bash
cd /c/Users/legam/Documents/btech/assignment/toc
ls src/  # Verify files are there
make clean && make
```

### Compilation Error: "dfa.c:45: undefined reference to `...`"

The optimized code might have an issue.

**Solution 1**: Clean build:
```bash
make clean
rm -rf obj
make
```

**Solution 2**: Check if files were properly modified:
```bash
grep "GROWTH_FACTOR" src/dfa.c    # Should find it
grep "string_pool" src/nfa.c      # Should find it
```

**Solution 3**: Verify optimization changes:
```bash
bash verify_optimization.sh  # Run verification script
```

### Runtime Error: "Segmentation fault"

Memory access issue (usually in the old executable).

**Solution**:
1. Rebuild fresh:
   ```bash
   make clean && make
   ```

2. Run the newly built version:
   ```bash
   ./dfa_converter
   ```

---

## Verifying Optimization

After successful build, verify optimizations were applied:

### 1. Check File I/O Optimization
```bash
grep "FILE_BUFFER_SIZE" src/main.c
grep "setvbuf" src/main.c
```

Should output:
```
#define FILE_BUFFER_SIZE 8192
setvbuf(file, buffer, _IOFBF, FILE_BUFFER_SIZE);
```

### 2. Check String Interning
```bash
grep "intern_string" src/dfa.c
grep "string_pool" src/nfa.c
```

Should find string interning functions.

### 3. Check Dynamic Allocation
```bash
grep "capacity_states" src/dfa.h
grep "int\* to_states" src/nfa.h
```

Should find dynamic allocation declarations.

### 4. Check Memory Cleanup
```bash
grep "free_nfa" src/nfa.c
grep "cleanup_examples" src/main.c
```

Should find proper cleanup code.

---

## Build Variants

### Rebuild Only (without cleaning)
```bash
make
```

### Full Clean Rebuild
```bash
make clean
make
```

### Run from Build
```bash
make run
```
(This runs the old test input; better to use `./dfa_converter` for interactive mode)

### Debug Build (with symbols for gdb)
```bash
make clean
CFLAGS="-Wall -std=c99 -g" make
gdb ./dfa_converter
```

---

## Performance Testing

After building, you can check memory usage improvements:

### Using /usr/bin/time (Linux/MSYS2)
```bash
/usr/bin/time -v ./dfa_converter < /dev/null
```

Shows memory and CPU usage.

### Using time command (Windows/MSYS2)
```bash
time echo "1" | ./dfa_converter > /dev/null
```

Shows execution time.

### Manual Memory Testing
```bash
echo "1" | ./dfa_converter
# Observe the conversion happens instantly
# Memory should be minimal (<1 MB)
```

---

## Next Steps

1. ✓ Build complete
2. ✓ Tests passing
3. Review documentation:
   - `OPTIMIZATION_SUMMARY.txt` - Overview
   - `OPTIMIZATION_CHANGES.md` - Detailed changes
   - `OPTIMIZATION_BEFORE_AFTER.txt` - Comparisons
4. Run custom DFA examples (edit `tests/dfa_examples.txt`)
5. Experiment with large examples (>100 states)

---

## Build Quick Reference

| Task | Command |
|------|---------|
| Clean | `make clean` |
| Build | `make` |
| Full rebuild | `make clean && make` |
| Run tests | `./dfa_converter` |
| View help | `make` |
| Remove everything | `make clean` |

---

## Troubleshooting Checklist

- [ ] In MSYS2 MinGW 64-bit terminal (not PowerShell)
- [ ] Correct directory: `/c/Users/legam/Documents/btech/assignment/toc`
- [ ] All tools installed: `gcc flex bison make`
- [ ] Run `make clean` first
- [ ] Run `make` to build
- [ ] Check for errors (should be none)
- [ ] Run `./dfa_converter` to test
- [ ] Select example 1 and verify output

---

## Support

If you still have issues:

1. Check `OPTIMIZATION_BUILD.md` for detailed setup
2. Review `SETUP_WINDOWS.md` for tool installation
3. See `README.md` for project overview
4. Check error messages carefully (usually clear about missing tools)

**Expected build time**: 5-15 seconds on modern computer

---

## Success Indicators

✓ Build completes with no errors
✓ Executable created: `dfa_converter`
✓ Runs without segmentation fault
✓ Displays menu with 10 examples
✓ Accepts user input 1-10
✓ Outputs DFA and NFA correctly
✓ Exits cleanly

**All indicators met = Optimization successful!**

