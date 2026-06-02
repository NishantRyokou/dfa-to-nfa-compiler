#include "nfa.h"

#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 1.5

/* String interning to avoid duplicate strings */
static const char* intern_string(NFA* nfa, const char* str) {
    if (!str || !str[0]) return "";
    
    size_t len = strlen(str);
    
    /* Check if already in pool */
    for (int i = 0; i < nfa->pool_size; i++) {
        if (strcmp(&nfa->string_pool[i], str) == 0) {
            return &nfa->string_pool[i];
        }
        i += strlen(&nfa->string_pool[i]) + 1;
    }
    
    /* Add to pool */
    if (nfa->pool_size + len + 1 > nfa->pool_capacity) {
        nfa->pool_capacity = (nfa->pool_capacity + len + 1) * GROWTH_FACTOR;
        char* new_pool = realloc(nfa->string_pool, nfa->pool_capacity);
        if (!new_pool) return NULL;
        nfa->string_pool = new_pool;
    }
    
    char* result = &nfa->string_pool[nfa->pool_size];
    strcpy(result, str);
    nfa->pool_size += len + 1;
    return result;
}

NFA* create_nfa() {
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    if (!nfa) return NULL;
    
    nfa->num_states = 0;
    nfa->capacity_states = INITIAL_CAPACITY;
    nfa->states = (State*)malloc(INITIAL_CAPACITY * sizeof(State));
    
    nfa->num_symbols = 0;
    nfa->capacity_symbols = INITIAL_CAPACITY;
    nfa->alphabet = (Symbol*)malloc(INITIAL_CAPACITY * sizeof(Symbol));
    
    nfa->num_final_states = 0;
    nfa->capacity_finals = INITIAL_CAPACITY;
    nfa->final_states = (State*)malloc(INITIAL_CAPACITY * sizeof(State));
    
    nfa->num_transitions = 0;
    nfa->capacity_transitions = INITIAL_CAPACITY * 2;
    nfa->transitions = (NFATransition*)malloc(INITIAL_CAPACITY * 2 * sizeof(NFATransition));
    
    nfa->pool_size = 0;
    nfa->pool_capacity = 256;
    nfa->string_pool = (char*)malloc(256);
    
    return nfa;
}

static void ensure_capacity_states(NFA* nfa) {
    if (nfa->num_states >= nfa->capacity_states) {
        nfa->capacity_states *= GROWTH_FACTOR;
        State* new_states = (State*)realloc(nfa->states, nfa->capacity_states * sizeof(State));
        if (new_states) nfa->states = new_states;
    }
}

static void ensure_capacity_finals(NFA* nfa) {
    if (nfa->num_final_states >= nfa->capacity_finals) {
        nfa->capacity_finals *= GROWTH_FACTOR;
        State* new_finals = (State*)realloc(nfa->final_states, nfa->capacity_finals * sizeof(State));
        if (new_finals) nfa->final_states = new_finals;
    }
}

static void ensure_capacity_transitions(NFA* nfa) {
    if (nfa->num_transitions >= nfa->capacity_transitions) {
        nfa->capacity_transitions *= GROWTH_FACTOR;
        NFATransition* new_trans = (NFATransition*)realloc(nfa->transitions, nfa->capacity_transitions * sizeof(NFATransition));
        if (new_trans) nfa->transitions = new_trans;
    }
}

void add_nfa_state(NFA* nfa, const char* name) {
    if (!nfa || !name) return;
    
    ensure_capacity_states(nfa);
    nfa->states[nfa->num_states].name = intern_string(nfa, name);
    nfa->num_states++;
}

void add_nfa_symbol(NFA* nfa, char symbol) {
    if (!nfa) return;
    if (nfa->num_symbols >= nfa->capacity_symbols) {
        nfa->capacity_symbols *= GROWTH_FACTOR;
        Symbol* new_alpha = (Symbol*)realloc(nfa->alphabet, nfa->capacity_symbols * sizeof(Symbol));
        if (new_alpha) nfa->alphabet = new_alpha;
    }
    nfa->alphabet[nfa->num_symbols].symbol = symbol;
    nfa->num_symbols++;
}

void set_nfa_initial_state(NFA* nfa, const char* name) {
    if (!nfa || !name) return;
    nfa->initial_state.name = intern_string(nfa, name);
}

void add_nfa_final_state(NFA* nfa, const char* name) {
    if (!nfa || !name) return;
    
    ensure_capacity_finals(nfa);
    nfa->final_states[nfa->num_final_states].name = intern_string(nfa, name);
    nfa->num_final_states++;
}

void add_nfa_transition(NFA* nfa, const char* from, char symbol, const char* to) {
    if (!nfa || !from || !to) return;
    
    /* Find or create transition for (from, symbol) pair */
    int trans_idx = -1;
    for (int i = 0; i < nfa->num_transitions; i++) {
        if (strcmp(nfa->transitions[i].from.name, from) == 0 && 
            nfa->transitions[i].symbol == symbol) {
            trans_idx = i;
            break;
        }
    }
    
    /* Create new transition if needed */
    if (trans_idx == -1) {
        ensure_capacity_transitions(nfa);
        trans_idx = nfa->num_transitions;
        nfa->transitions[trans_idx].from.name = intern_string(nfa, from);
        nfa->transitions[trans_idx].symbol = symbol;
        nfa->transitions[trans_idx].num_to_states = 0;
        nfa->transitions[trans_idx].capacity_to_states = INITIAL_CAPACITY;
        nfa->transitions[trans_idx].to_states = (int*)malloc(INITIAL_CAPACITY * sizeof(int));
        nfa->num_transitions++;
    }
    
    /* Find state index for 'to' */
    int state_idx = -1;
    for (int i = 0; i < nfa->num_states; i++) {
        if (strcmp(nfa->states[i].name, to) == 0) {
            state_idx = i;
            break;
        }
    }
    
    if (state_idx != -1) {
        /* Check if not already added */
        int found = 0;
        for (int i = 0; i < nfa->transitions[trans_idx].num_to_states; i++) {
            if (nfa->transitions[trans_idx].to_states[i] == state_idx) {
                found = 1;
                break;
            }
        }
        
        if (!found) {
            /* Add to target states */
            if (nfa->transitions[trans_idx].num_to_states >= nfa->transitions[trans_idx].capacity_to_states) {
                nfa->transitions[trans_idx].capacity_to_states *= GROWTH_FACTOR;
                int* new_targets = (int*)realloc(nfa->transitions[trans_idx].to_states, 
                                                  nfa->transitions[trans_idx].capacity_to_states * sizeof(int));
                if (new_targets) nfa->transitions[trans_idx].to_states = new_targets;
            }
            nfa->transitions[trans_idx].to_states[nfa->transitions[trans_idx].num_to_states] = state_idx;
            nfa->transitions[trans_idx].num_to_states++;
        }
    }
}

void print_nfa(NFA* nfa) {
    if (!nfa) return;
    
    printf("=== NFA ===\n");
    printf("States: ");
    for (int i = 0; i < nfa->num_states; i++) {
        printf("%s ", nfa->states[i].name);
    }
    printf("\n");
    
    printf("Alphabet: ");
    for (int i = 0; i < nfa->num_symbols; i++) {
        printf("%c ", nfa->alphabet[i].symbol);
    }
    printf("\n");
    
    printf("Initial State: %s\n", nfa->initial_state.name);
    
    printf("Final States: ");
    for (int i = 0; i < nfa->num_final_states; i++) {
        printf("%s ", nfa->final_states[i].name);
    }
    printf("\n");
    
    printf("Transitions:\n");
    for (int i = 0; i < nfa->num_transitions; i++) {
        printf("  %s --(%c)--> ", 
            nfa->transitions[i].from.name,
            nfa->transitions[i].symbol);
        for (int j = 0; j < nfa->transitions[i].num_to_states; j++) {
            if (j > 0) printf(", ");
            printf("%s", nfa->states[nfa->transitions[i].to_states[j]].name);
        }
        printf("\n");
    }
}

void free_nfa(NFA* nfa) {
    if (!nfa) return;
    
    /* Free dynamically allocated transition target arrays */
    for (int i = 0; i < nfa->num_transitions; i++) {
        free(nfa->transitions[i].to_states);
    }
    
    free(nfa->states);
    free(nfa->alphabet);
    free(nfa->final_states);
    free(nfa->transitions);
    free(nfa->string_pool);
    free(nfa);
}
