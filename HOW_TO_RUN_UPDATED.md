# How to Run: DFA ↔ NFA Bidirectional Converter

## Quick Start (3 Steps)

### Step 1: Open Command Prompt
- Press `Windows Key + R`
- Type `cmd`
- Press `Enter`

### Step 2: Navigate to Project
```cmd
cd C:\Users\legam\Documents\btech\assignment\toc
```

### Step 3: Run the Program
```cmd
dfa_converter.exe
```

---

## What You'll See

### Mode Selection Menu
```
╔════════════════════════════════════════╗
║   DFA ↔ NFA Bidirectional Converter   ║
║      Using Flex and Bison              ║
╚════════════════════════════════════════╝

Select Conversion Mode:
  [1] DFA to NFA (Simple Conversion)
  [2] NFA to DFA (Subset Construction Algorithm)

Enter your choice (1-2):
```

---

## Mode 1: DFA to NFA Conversion

### What It Does
Converts a deterministic finite automaton to a non-deterministic one (simple copy).

### Example

**Input:** Type `1` then select example `1`

**Input DFA:**
```
States: q0, q1, q2
Alphabet: a, b
Initial State: q0
Final States: q2
Transitions:
  q0 --(a)--> q1
  q1 --(b)--> q2
```

**Output NFA:**
```
States: q0, q1, q2
Alphabet: a, b
Initial State: q0
Final States: q2
Transitions:
  q0 --(a)--> q1
  q1 --(b)--> q2
```

**Result:** Identical (DFA is already deterministic)

---

## Mode 2: NFA to DFA Conversion ⭐ NEW!

### What It Does
Converts a non-deterministic automaton to deterministic using **subset construction algorithm**.

### Why This Is Important
- Shows **state explosion** (NFA states combine)
- Demonstrates **real difference** between NFA and DFA
- Uses **powerset construction** algorithm
- This is what makes the project academically valuable!

### Example

**Input:** Type `2` then select example `1`

**Input NFA:**
```
States: q0, q1, q2, q3
Alphabet: a, b
Initial State: q0
Final States: q3
Transitions:
  q0 --(a)--> q0, q1    ← Goes to MULTIPLE states!
  q1 --(b)--> q2
  q2 --(b)--> q3
```

**Output DFA:**
```
States: {q0}, {q0,q1}, {q2}, {q3}    ← Combined states!
Alphabet: a, b
Initial State: {q0}
Final States: {q3}
Transitions:
  {q0} --(a)--> {q0,q1}      ← Combined state!
  {q0,q1} --(a)--> {q0,q1}
  {q0,q1} --(b)--> {q2}
  {q2} --(b)--> {q3}
```

**Key Observation:**
- NFA allows `q0 --(a)--> q0, q1` (non-deterministic!)
- DFA creates `{q0,q1}` state to track both possibilities
- This is **subset construction** in action!

---

## All Available Examples

### Mode 1: DFA Examples (10 total)
1. 3 states, 2 symbols (a, b)
2. 4 states, 2 symbols (0, 1)
3. 3 states, 2 symbols (x, y)
4. 6 states, 1 symbol (a)
5. 7 states, 3 symbols (0, 1, 2)
6. 5 states, 3 symbols (c, d, e)
7. 6 states, 2 symbols (i, j)
8. 7 states, 3 symbols (u, v, w)
9. 8 states, 1 symbol (k)
10. 7 states, 2 symbols (l, m)

### Mode 2: NFA Examples (5 total) ⭐ NEW!
1. **4 states** - Non-deterministic loop with 'a'
2. **3 states** - Binary alphabet with multiple transitions
3. **4 states** - Multiple final states
4. **3 states** - Triple transitions on same symbol
5. **5 states** - Complex path with branching

---

## Interactive Session Example

```
C:\Users\legam\Documents\btech\assignment\toc> dfa_converter.exe

╔════════════════════════════════════════╗
║   DFA ↔ NFA Bidirectional Converter   ║
║      Using Flex and Bison              ║
╚════════════════════════════════════════╝

Select Conversion Mode:
  [1] DFA to NFA (Simple Conversion)
  [2] NFA to DFA (Subset Construction Algorithm)

Enter your choice (1-2): 2

Successfully loaded 5 examples.

========================================
Available NFA Examples:
========================================

[1] NFA { states: q0, q1, q2, q3 ... }
[2] NFA { states: s0, s1, s2 ... }
[3] NFA { states: p0, p1, p2, p3 ... }
[4] NFA { states: r0, r1, r2 ... }
[5] NFA { states: t0, t1, t2, t3, t4 ... }

Enter the number of the NFA example to convert (1-5): 1

========================================
Converting Example 1
========================================

NFA parsed successfully

>>> INPUT NFA <<<
[Shows the non-deterministic automaton]

>>> CONVERTED DFA (using Subset Construction) <<<
[Shows the deterministic automaton with combined states]

--- Notice how multiple NFA states combine into single DFA states! ---

========================================
Conversion Complete!
========================================
```

---

## Automated Testing

### Test Mode 2 Automatically
```cmd
test_nfa_to_dfa.bat
```

This runs Mode 2 with Example 1 automatically.

---

## Files You Need

### To RUN the program:
- ✅ `dfa_converter.exe` (the executable)
- ✅ `tests/dfa_examples.txt` (for Mode 1)
- ✅ `tests/nfa_examples.txt` (for Mode 2) ⭐ NEW!

### To BUILD the program:
- All source files in `src/`
- MSYS2 with gcc, flex, bison
- Run `build_msys2.bat`

---

## Common Questions

### Q: Why do Mode 1 outputs look identical?
**A:** DFAs are already deterministic! The conversion just copies the structure. This is correct behavior.

### Q: Why do Mode 2 states have weird names like `{q0,q1}`?
**A:** That's subset construction! The DFA creates combined states to track multiple NFA states simultaneously.

### Q: Which mode should I focus on for my assignment?
**A:** **Mode 2** (NFA → DFA) is more impressive! It shows:
- Real transformation algorithm
- State explosion concept
- Complexity trade-offs
- Theoretical concepts in practice

### Q: What's the difference between the two modes?
**A:** 
- **Mode 1 (DFA→NFA)**: Simple, output identical to input
- **Mode 2 (NFA→DFA)**: Complex algorithm, creates combined states, shows real transformation

---

## Tips for Your Assignment Report

### Include These Points:

1. **Both Conversions Implemented**
   - DFA → NFA: O(n) complexity
   - NFA → DFA: O(2^n) complexity (subset construction)

2. **Show Examples**
   - Run Mode 2 with different examples
   - Screenshot the combined states
   - Explain why `{q0,q1}` represents both states

3. **Explain Subset Construction**
   - Groups of NFA states → Single DFA states
   - Eliminates non-determinism
   - May cause state explosion

4. **Discuss Trade-offs**
   - NFA: Compact, easy to design, non-deterministic
   - DFA: Larger, harder to design, fast execution
   - Conversion allows best of both worlds

---

## What Makes This Special

✅ **Complete Compiler Pipeline**
- Lexical Analysis (Flex)
- Syntax Analysis (Bison)
- Semantic Analysis (Automaton construction)
- Code Generation (Subset construction)

✅ **Real Algorithms**
- DFA→NFA: Direct mapping
- NFA→DFA: Powerset construction

✅ **Visible Results**
- See non-determinism
- See state combinations
- See transformations

✅ **Academic Value**
- Theory to practice
- Algorithms in action
- Real compiler design

---

## Troubleshooting

### Problem: "flex not found"
**Solution:** Use `build_msys2.bat` which uses MSYS2 tools directly

### Problem: Program hangs after selecting mode
**Solution:** You need to select an example number. Type a number (1-10 for Mode 1, 1-5 for Mode 2)

### Problem: "No examples loaded"
**Solution:** Make sure `tests/dfa_examples.txt` and `tests/nfa_examples.txt` exist

### Problem: Parsing failed
**Solution:** Check example format - transitions need proper spacing around `--` and `->`

---

## Ready to Run?

1. Open Command Prompt
2. Navigate to project: `cd C:\Users\legam\Documents\btech\assignment\toc`
3. Run: `dfa_converter.exe`
4. Select Mode 2 (NFA to DFA)
5. Choose Example 1
6. Watch the magic happen! ✨

---

**Version**: 2.0  
**Date**: June 3, 2026  
**Status**: ✅ Fully Functional  
**New Feature**: NFA → DFA Conversion with Subset Construction
