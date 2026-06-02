#ifndef DFA_H
#define DFA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_ALPHABET 50
#define MAX_NAME 50
#define MAX_TRANSITIONS (MAX_STATES * MAX_ALPHABET)

/* Use pointers for better memory efficiency and cache locality */
typedef struct {
    const char* name;  /* Points to interned string */
} State;

typedef struct {
    char symbol;
} Symbol;

typedef struct {
    State from;
    char symbol;
    State to;
} Transition;

typedef struct {
    State* states;           /* Dynamic allocation for actual data */
    int num_states;
    int capacity_states;
    
    Symbol* alphabet;        /* Dynamic allocation */
    int num_symbols;
    int capacity_symbols;
    
    State initial_state;
    State* final_states;     /* Dynamic allocation */
    int num_final_states;
    int capacity_finals;
    
    Transition* transitions; /* Dynamic allocation */
    int num_transitions;
    int capacity_transitions;
    
    char* string_pool;       /* String interning for efficiency */
    int pool_size;
    int pool_capacity;
} DFA;

/* Function prototypes */
DFA* create_dfa(void);
void add_state(DFA* dfa, const char* name);
void add_symbol(DFA* dfa, char symbol);
void set_initial_state(DFA* dfa, const char* name);
void add_final_state(DFA* dfa, const char* name);
void add_transition(DFA* dfa, const char* from, char symbol, const char* to);
void print_dfa(const DFA* dfa);
void free_dfa(DFA* dfa);

#endif
