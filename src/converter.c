#include "converter.h"
#include <string.h>

/* ========================================================================
   DFA to NFA Conversion (Simple - preserves determinism)
   ======================================================================== */

NFA* convert_dfa_to_nfa(DFA* dfa) {
    NFA* nfa = create_nfa();
    
    for (int i = 0; i < dfa->num_states; i++) {
        add_nfa_state(nfa, dfa->states[i].name);
    }
    
    for (int i = 0; i < dfa->num_symbols; i++) {
        add_nfa_symbol(nfa, dfa->alphabet[i].symbol);
    }
    
    set_nfa_initial_state(nfa, dfa->initial_state.name);
    
    for (int i = 0; i < dfa->num_final_states; i++) {
        add_nfa_final_state(nfa, dfa->final_states[i].name);
    }
    
    for (int i = 0; i < dfa->num_transitions; i++) {
        add_nfa_transition(nfa, 
            dfa->transitions[i].from.name,
            dfa->transitions[i].symbol,
            dfa->transitions[i].to.name);
    }
    
    return nfa;
}

/* ========================================================================
   NFA to DFA Conversion (Subset Construction / Powerset Algorithm)
   ======================================================================== */

#define MAX_SUBSET_STATES 1000
#define MAX_STATE_NAME 256

/* Represents a set of NFA states (becomes one DFA state) */
typedef struct {
    int* state_indices;     /* Array of NFA state indices in this subset */
    int num_states;
    char name[MAX_STATE_NAME];  /* Name like "{q0,q1,q2}" */
} StateSubset;

/* Helper: Check if a state index is in the subset */
static int subset_contains(StateSubset* subset, int state_idx) {
    for (int i = 0; i < subset->num_states; i++) {
        if (subset->state_indices[i] == state_idx) {
            return 1;
        }
    }
    return 0;
}

/* Helper: Compare two subsets for equality */
static int subsets_equal(StateSubset* s1, StateSubset* s2) {
    if (s1->num_states != s2->num_states) return 0;
    
    /* Check if all states in s1 are in s2 */
    for (int i = 0; i < s1->num_states; i++) {
        if (!subset_contains(s2, s1->state_indices[i])) {
            return 0;
        }
    }
    return 1;
}

/* Helper: Create name for subset like "{q0,q1}" */
static void create_subset_name(StateSubset* subset, NFA* nfa) {
    if (subset->num_states == 0) {
        strcpy(subset->name, "{}");
        return;
    }
    
    subset->name[0] = '{';
    subset->name[1] = '\0';
    
    for (int i = 0; i < subset->num_states; i++) {
        if (i > 0) strcat(subset->name, ",");
        strcat(subset->name, nfa->states[subset->state_indices[i]].name);
    }
    strcat(subset->name, "}");
}

/* Helper: Compute transition from subset on symbol */
static void compute_move(NFA* nfa, StateSubset* from_subset, char symbol, StateSubset* result) {
    result->num_states = 0;
    
    /* For each state in the from_subset */
    for (int i = 0; i < from_subset->num_states; i++) {
        int from_state = from_subset->state_indices[i];
        
        /* Find transitions from this state on this symbol */
        for (int t = 0; t < nfa->num_transitions; t++) {
            /* Check if transition matches */
            int trans_from = -1;
            for (int s = 0; s < nfa->num_states; s++) {
                if (strcmp(nfa->states[s].name, nfa->transitions[t].from.name) == 0) {
                    trans_from = s;
                    break;
                }
            }
            
            if (trans_from == from_state && nfa->transitions[t].symbol == symbol) {
                /* Add all target states to result */
                for (int j = 0; j < nfa->transitions[t].num_to_states; j++) {
                    int target = nfa->transitions[t].to_states[j];
                    if (!subset_contains(result, target)) {
                        result->state_indices[result->num_states++] = target;
                    }
                }
            }
        }
    }
    
    create_subset_name(result, nfa);
}

/* Helper: Check if subset contains any final state */
static int is_final_subset(NFA* nfa, StateSubset* subset) {
    for (int i = 0; i < subset->num_states; i++) {
        int state_idx = subset->state_indices[i];
        const char* state_name = nfa->states[state_idx].name;
        
        /* Check if this state is final in NFA */
        for (int f = 0; f < nfa->num_final_states; f++) {
            if (strcmp(state_name, nfa->final_states[f].name) == 0) {
                return 1;
            }
        }
    }
    return 0;
}

/* Main conversion function: NFA to DFA using subset construction */
DFA* convert_nfa_to_dfa(NFA* nfa) {
    if (!nfa) return NULL;
    
    DFA* dfa = create_dfa();
    
    /* Copy alphabet */
    for (int i = 0; i < nfa->num_symbols; i++) {
        add_symbol(dfa, nfa->alphabet[i].symbol);
    }
    
    /* Array of subsets (DFA states) */
    StateSubset* subsets = malloc(MAX_SUBSET_STATES * sizeof(StateSubset));
    for (int i = 0; i < MAX_SUBSET_STATES; i++) {
        subsets[i].state_indices = malloc(nfa->num_states * sizeof(int));
    }
    int num_subsets = 0;
    
    /* Start with initial state subset {q0} */
    int initial_idx = -1;
    for (int i = 0; i < nfa->num_states; i++) {
        if (strcmp(nfa->states[i].name, nfa->initial_state.name) == 0) {
            initial_idx = i;
            break;
        }
    }
    
    if (initial_idx == -1) {
        /* Cleanup and return */
        for (int i = 0; i < MAX_SUBSET_STATES; i++) {
            free(subsets[i].state_indices);
        }
        free(subsets);
        return dfa;
    }
    
    /* Create initial subset */
    subsets[0].num_states = 1;
    subsets[0].state_indices[0] = initial_idx;
    create_subset_name(&subsets[0], nfa);
    num_subsets = 1;
    
    add_state(dfa, subsets[0].name);
    set_initial_state(dfa, subsets[0].name);
    
    if (is_final_subset(nfa, &subsets[0])) {
        add_final_state(dfa, subsets[0].name);
    }
    
    /* Process each subset */
    int processed = 0;
    while (processed < num_subsets) {
        StateSubset* current = &subsets[processed];
        
        /* For each symbol in alphabet */
        for (int sym = 0; sym < nfa->num_symbols; sym++) {
            char symbol = nfa->alphabet[sym].symbol;
            
            /* Compute move(current, symbol) */
            StateSubset result;
            result.state_indices = malloc(nfa->num_states * sizeof(int));
            compute_move(nfa, current, symbol, &result);
            
            if (result.num_states > 0) {
                /* Check if this subset already exists */
                int found_idx = -1;
                for (int i = 0; i < num_subsets; i++) {
                    if (subsets_equal(&result, &subsets[i])) {
                        found_idx = i;
                        break;
                    }
                }
                
                /* If new subset, add it */
                if (found_idx == -1) {
                    if (num_subsets < MAX_SUBSET_STATES) {
                        /* Copy result to subsets array */
                        subsets[num_subsets].num_states = result.num_states;
                        for (int i = 0; i < result.num_states; i++) {
                            subsets[num_subsets].state_indices[i] = result.state_indices[i];
                        }
                        strcpy(subsets[num_subsets].name, result.name);
                        
                        add_state(dfa, subsets[num_subsets].name);
                        
                        if (is_final_subset(nfa, &subsets[num_subsets])) {
                            add_final_state(dfa, subsets[num_subsets].name);
                        }
                        
                        found_idx = num_subsets;
                        num_subsets++;
                    }
                }
                
                /* Add transition */
                if (found_idx != -1) {
                    add_transition(dfa, current->name, symbol, subsets[found_idx].name);
                }
            }
            
            free(result.state_indices);
        }
        
        processed++;
    }
    
    /* Cleanup */
    for (int i = 0; i < MAX_SUBSET_STATES; i++) {
        free(subsets[i].state_indices);
    }
    free(subsets);
    
    return dfa;
}
