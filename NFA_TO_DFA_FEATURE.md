# NFA to DFA Conversion Feature

## Overview

The program now supports **bidirectional conversion**:
- **Mode 1**: DFA → NFA (simple conversion, preserves determinism)
- **Mode 2**: NFA → DFA (subset construction algorithm, shows state explosion!)

## What's New

### NFA to DFA Conversion (Subset Construction Algorithm)

This is the **real transformation** that demonstrates the difference between NFA and DFA!

#### How It Works

1. **Non-deterministic NFA**: Multiple transitions possible from one state on same symbol
   ```
   q0 --(a)--> q0, q1    ← Goes to TWO states!
   ```

2. **Deterministic DFA**: Each state+symbol combination has exactly ONE target
   ```
   {q0} --(a)--> {q0,q1}    ← Combined state!
   ```

3. **Subset Construction**: Groups of NFA states become single DFA states
   - `{q0}` - just state q0
   - `{q0,q1}` - both q0 AND q1 simultaneously
   - `{q2}` - just state q2
   - etc.

## Running the Program

### Quick Start

```cmd
dfa_converter.exe
```

### Interactive Menu

```
Select Conversion Mode:
  [1] DFA to NFA (Simple Conversion)
  [2] NFA to DFA (Subset Construction Algorithm)

Enter your choice (1-2): 2

[Select an NFA example from the list]
```

## Example Results

### Example 1: NFA with Non-determinism

**Input NFA:**
```
States: q0, q1, q2, q3
Alphabet: a, b
Initial: q0
Finals: q3
Transitions:
  q0 --(a)--> q0, q1    ← NON-DETERMINISTIC!
  q1 --(b)--> q2
  q2 --(b)--> q3
```

**Output DFA:**
```
States: {q0}, {q0,q1}, {q2}, {q3}
Alphabet: a, b
Initial: {q0}
Finals: {q3}
Transitions:
  {q0} --(a)--> {q0,q1}      ← Combined state!
  {q0,q1} --(a)--> {q0,q1}   ← Self-loop
  {q0,q1} --(b)--> {q2}
  {q2} --(b)--> {q3}
```

**Key Difference**: NFA state q0 can go to **both q0 AND q1** on symbol 'a', 
so DFA creates combined state `{q0,q1}` to track both possibilities!

## Test Files

### NFA Examples
- Location: `tests/nfa_examples.txt`
- Contains: 5 NFAs with non-deterministic transitions
- Format: `NFA { states: ... alphabet: ... initial: ... finals: ... transitions: ... }`

### DFA Examples  
- Location: `tests/dfa_examples.txt`
- Contains: 10 DFAs (original examples)
- Format: `DFA { states: ... alphabet: ... initial: ... finals: ... transitions: ... }`

## Algorithm Details

### Subset Construction (Powerset Algorithm)

1. **Start**: Initial DFA state = {NFA initial state}
2. **Process each DFA state**:
   - For each symbol in alphabet:
     - Find all NFA states reachable from any state in current subset
     - Create new DFA state for this subset (if not already exists)
     - Add transition from current to new state
3. **Final states**: DFA states containing any NFA final state
4. **Result**: Deterministic automaton equivalent to original NFA

### Why This Matters

- **NFA**: Easy to design, non-deterministic (multiple choices)
- **DFA**: Hard to design, deterministic (no choices)
- **Conversion**: Automatically transforms NFA → DFA
- **Trade-off**: May create many more states (state explosion)

## Building

### Windows (MSYS2)

```cmd
build_msys2.bat
```

### Requirements
- MSYS2 with flex, bison, gcc
- Located at: `C:\msys64\`

## Testing

### Automated Test

```cmd
test_nfa_to_dfa.bat
```

Runs Mode 2 with Example 1 automatically.

### Manual Testing

1. Run `dfa_converter.exe`
2. Select Mode 2
3. Choose any NFA example (1-5)
4. Observe how NFA states combine in DFA output

## Key Features

✅ **Bidirectional Conversion**
- DFA → NFA: Simple copy
- NFA → DFA: Subset construction

✅ **Non-determinism Handling**
- Detects multiple targets per (state, symbol)
- Creates combined states automatically
- Maintains semantic equivalence

✅ **State Explosion Demonstration**
- Shows how NFAs can be more compact
- Illustrates DFA state explosion
- Real-world compiler design concept

✅ **Complete Implementation**
- Full lexer and parser for both DFA and NFA
- Optimized memory management
- Clean output formatting

## Academic Value

This demonstrates:
1. **Lexical Analysis**: Flex recognizes DFA/NFA keywords
2. **Syntax Analysis**: Bison parses specifications
3. **Semantic Analysis**: Builds automaton data structures
4. **Code Generation**: Subset construction algorithm
5. **Optimization**: String interning, dynamic allocation
6. **Theory to Practice**: Formal automata → working code

## Files Modified

- `src/converter.h` - Added `convert_nfa_to_dfa()` declaration
- `src/converter.c` - Implemented subset construction algorithm
- `src/main.c` - Added mode selection and NFA loading
- `src/parser.y` - Added NFA grammar rules
- `src/lexer.l` - Added NFA keyword recognition
- `tests/nfa_examples.txt` - Created 5 NFA test cases

## Comparison with DFA → NFA

| Aspect | DFA → NFA | NFA → DFA |
|--------|-----------|-----------|
| Complexity | O(n) simple copy | O(2^n) subset construction |
| States | Same count | May increase (state explosion) |
| Output | Identical structure | Combined states |
| Algorithm | Direct mapping | Powerset construction |
| Educational Value | Minimal | High (shows real transformation) |

## Next Steps

To use this for your assignment:

1. **Run both modes** to compare outputs
2. **Document the differences** in your report
3. **Explain subset construction** algorithm
4. **Show state explosion** with examples
5. **Discuss complexity** trade-offs

## Questions?

- Mode 1 outputs look identical? → **Correct!** DFAs are already deterministic
- Mode 2 creates weird state names like `{q0,q1}`? → **Correct!** That's subset construction!
- Why do I need this? → Shows the **real difference** between NFA and DFA for your assignment!

---

**Date Added**: June 3, 2026  
**Version**: 2.0  
**Status**: ✅ Complete and Working
