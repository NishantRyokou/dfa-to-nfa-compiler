#ifndef NFA_H
#define NFA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfa.h"

#define MAX_STATES 100
#define MAX_ALPHABET 50

/* For NFA, each transition can have multiple target states */
typedef struct {
    State from;
    char symbol;
    int* to_states;           /* Dynamic array of state indices */
    int num_to_states;
    int capacity_to_states;
} NFATransition;

typedef struct {
    State* states;            /* Dynamic allocation */
    int num_states;
    int capacity_states;
    
    Symbol* alphabet;         /* Dynamic allocation */
    int num_symbols;
    int capacity_symbols;
    
    State initial_state;
    State* final_states;      /* Dynamic allocation */
    int num_final_states;
    int capacity_finals;
    
    NFATransition* transitions; /* Dynamic allocation */
    int num_transitions;
    int capacity_transitions;
    
    char* string_pool;        /* String interning for efficiency */
    int pool_size;
    int pool_capacity;
} NFA;

NFA* create_nfa();
void add_nfa_state(NFA* nfa, const char* name);
void add_nfa_symbol(NFA* nfa, char symbol);
void set_nfa_initial_state(NFA* nfa, const char* name);
void add_nfa_final_state(NFA* nfa, const char* name);
void add_nfa_transition(NFA* nfa, const char* from, char symbol, const char* to);
void print_nfa(NFA* nfa);
void free_nfa(NFA* nfa);

#endif
