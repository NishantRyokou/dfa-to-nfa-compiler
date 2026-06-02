#include "dfa.h"

#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 1.5

/* String interning to avoid duplicate strings */
static const char* intern_string(DFA* dfa, const char* str) {
    if (!str || !str[0]) return "";
    
    size_t len = strlen(str);
    
    /* Check if already in pool */
    for (int i = 0; i < dfa->pool_size; i++) {
        if (strcmp(&dfa->string_pool[i], str) == 0) {
            return &dfa->string_pool[i];
        }
        i += strlen(&dfa->string_pool[i]) + 1;
    }
    
    /* Add to pool */
    if (dfa->pool_size + len + 1 > dfa->pool_capacity) {
        dfa->pool_capacity = (dfa->pool_capacity + len + 1) * GROWTH_FACTOR;
        char* new_pool = realloc(dfa->string_pool, dfa->pool_capacity);
        if (!new_pool) return NULL;
        dfa->string_pool = new_pool;
    }
    
    char* result = &dfa->string_pool[dfa->pool_size];
    strcpy(result, str);
    dfa->pool_size += len + 1;
    return result;
}

DFA* create_dfa(void) {
    DFA* dfa = (DFA*)malloc(sizeof(DFA));
    if (!dfa) return NULL;
    
    dfa->num_states = 0;
    dfa->capacity_states = INITIAL_CAPACITY;
    dfa->states = (State*)malloc(INITIAL_CAPACITY * sizeof(State));
    
    dfa->num_symbols = 0;
    dfa->capacity_symbols = INITIAL_CAPACITY;
    dfa->alphabet = (Symbol*)malloc(INITIAL_CAPACITY * sizeof(Symbol));
    
    dfa->num_final_states = 0;
    dfa->capacity_finals = INITIAL_CAPACITY;
    dfa->final_states = (State*)malloc(INITIAL_CAPACITY * sizeof(State));
    
    dfa->num_transitions = 0;
    dfa->capacity_transitions = INITIAL_CAPACITY * 2;
    dfa->transitions = (Transition*)malloc(INITIAL_CAPACITY * 2 * sizeof(Transition));
    
    dfa->pool_size = 0;
    dfa->pool_capacity = 256;
    dfa->string_pool = (char*)malloc(256);
    
    return dfa;
}

static void ensure_capacity_states(DFA* dfa) {
    if (dfa->num_states >= dfa->capacity_states) {
        dfa->capacity_states *= GROWTH_FACTOR;
        State* new_states = (State*)realloc(dfa->states, dfa->capacity_states * sizeof(State));
        if (new_states) dfa->states = new_states;
    }
}

static void ensure_capacity_finals(DFA* dfa) {
    if (dfa->num_final_states >= dfa->capacity_finals) {
        dfa->capacity_finals *= GROWTH_FACTOR;
        State* new_finals = (State*)realloc(dfa->final_states, dfa->capacity_finals * sizeof(State));
        if (new_finals) dfa->final_states = new_finals;
    }
}

static void ensure_capacity_transitions(DFA* dfa) {
    if (dfa->num_transitions >= dfa->capacity_transitions) {
        dfa->capacity_transitions *= GROWTH_FACTOR;
        Transition* new_trans = (Transition*)realloc(dfa->transitions, dfa->capacity_transitions * sizeof(Transition));
        if (new_trans) dfa->transitions = new_trans;
    }
}

void add_state(DFA* dfa, const char* name) {
    if (!dfa || !name) return;
    
    ensure_capacity_states(dfa);
    dfa->states[dfa->num_states].name = intern_string(dfa, name);
    dfa->num_states++;
}

void add_symbol(DFA* dfa, char symbol) {
    if (!dfa) return;
    if (dfa->num_symbols >= dfa->capacity_symbols) {
        dfa->capacity_symbols *= GROWTH_FACTOR;
        Symbol* new_alpha = (Symbol*)realloc(dfa->alphabet, dfa->capacity_symbols * sizeof(Symbol));
        if (new_alpha) dfa->alphabet = new_alpha;
    }
    dfa->alphabet[dfa->num_symbols].symbol = symbol;
    dfa->num_symbols++;
}

void set_initial_state(DFA* dfa, const char* name) {
    if (!dfa || !name) return;
    dfa->initial_state.name = intern_string(dfa, name);
}

void add_final_state(DFA* dfa, const char* name) {
    if (!dfa || !name) return;
    
    ensure_capacity_finals(dfa);
    dfa->final_states[dfa->num_final_states].name = intern_string(dfa, name);
    dfa->num_final_states++;
}

void add_transition(DFA* dfa, const char* from, char symbol, const char* to) {
    if (!dfa || !from || !to) return;
    
    ensure_capacity_transitions(dfa);
    dfa->transitions[dfa->num_transitions].from.name = intern_string(dfa, from);
    dfa->transitions[dfa->num_transitions].symbol = symbol;
    dfa->transitions[dfa->num_transitions].to.name = intern_string(dfa, to);
    dfa->num_transitions++;
}

void print_dfa(const DFA* dfa) {
    if (!dfa) return;
    
    printf("=== DFA ===\n");
    printf("States: ");
    for (int i = 0; i < dfa->num_states; i++) {
        printf("%s ", dfa->states[i].name);
    }
    printf("\n");
    
    printf("Alphabet: ");
    for (int i = 0; i < dfa->num_symbols; i++) {
        printf("%c ", dfa->alphabet[i].symbol);
    }
    printf("\n");
    
    printf("Initial State: %s\n", dfa->initial_state.name);
    
    printf("Final States: ");
    for (int i = 0; i < dfa->num_final_states; i++) {
        printf("%s ", dfa->final_states[i].name);
    }
    printf("\n");
    
    printf("Transitions:\n");
    for (int i = 0; i < dfa->num_transitions; i++) {
        printf("  %s --(%c)--> %s\n", 
            dfa->transitions[i].from.name,
            dfa->transitions[i].symbol,
            dfa->transitions[i].to.name);
    }
}

void free_dfa(DFA* dfa) {
    if (!dfa) return;
    free(dfa->states);
    free(dfa->alphabet);
    free(dfa->final_states);
    free(dfa->transitions);
    free(dfa->string_pool);
    free(dfa);
}
