#include "converter.h"
#include <string.h>

/* DFA to NFA: Simple copy */
NFA* convert_dfa_to_nfa(DFA* dfa) {
    NFA* nfa = create_nfa();
    int i;
    
    for (i = 0; i < dfa->num_states; i++) add_nfa_state(nfa, dfa->states[i].name);
    for (i = 0; i < dfa->num_symbols; i++) add_nfa_symbol(nfa, dfa->alphabet[i].symbol);
    for (i = 0; i < dfa->num_final_states; i++) add_nfa_final_state(nfa, dfa->final_states[i].name);
    for (i = 0; i < dfa->num_transitions; i++) {
        add_nfa_transition(nfa, dfa->transitions[i].from.name, 
                          dfa->transitions[i].symbol, dfa->transitions[i].to.name);
    }
    
    set_nfa_initial_state(nfa, dfa->initial_state.name);
    return nfa;
}

/* NFA to DFA: Subset construction algorithm */
#define MAX_SUBSETS 500
#define SUBSET_NAME_SIZE 256

typedef struct {
    int* states;
    int count;
    char name[SUBSET_NAME_SIZE];
} Subset;

static int contains(Subset* s, int idx) {
    for (int i = 0; i < s->count; i++) if (s->states[i] == idx) return 1;
    return 0;
}

static int equals(Subset* a, Subset* b) {
    if (a->count != b->count) return 0;
    for (int i = 0; i < a->count; i++) if (!contains(b, a->states[i])) return 0;
    return 1;
}

static void make_name(Subset* s, NFA* nfa) {
    if (!s->count) { strcpy(s->name, "{}"); return; }
    strcpy(s->name, "{");
    for (int i = 0; i < s->count; i++) {
        if (i) strcat(s->name, ",");
        strcat(s->name, nfa->states[s->states[i]].name);
    }
    strcat(s->name, "}");
}

static void move(NFA* nfa, Subset* from, char sym, Subset* to) {
    to->count = 0;
    for (int i = 0; i < from->count; i++) {
        for (int t = 0; t < nfa->num_transitions; t++) {
            int f = -1;
            for (int s = 0; s < nfa->num_states; s++) {
                if (!strcmp(nfa->states[s].name, nfa->transitions[t].from.name)) { f = s; break; }
            }
            if (f == from->states[i] && nfa->transitions[t].symbol == sym) {
                for (int j = 0; j < nfa->transitions[t].num_to_states; j++) {
                    int tgt = nfa->transitions[t].to_states[j];
                    if (!contains(to, tgt)) to->states[to->count++] = tgt;
                }
            }
        }
    }
    make_name(to, nfa);
}

static int is_final(NFA* nfa, Subset* s) {
    for (int i = 0; i < s->count; i++) {
        for (int f = 0; f < nfa->num_final_states; f++) {
            if (!strcmp(nfa->states[s->states[i]].name, nfa->final_states[f].name)) return 1;
        }
    }
    return 0;
}

DFA* convert_nfa_to_dfa(NFA* nfa) {
    if (!nfa) return NULL;
    DFA* dfa = create_dfa();
    int i, init = -1;
    
    for (i = 0; i < nfa->num_symbols; i++) add_symbol(dfa, nfa->alphabet[i].symbol);
    for (i = 0; i < nfa->num_states; i++) {
        if (!strcmp(nfa->states[i].name, nfa->initial_state.name)) { init = i; break; }
    }
    if (init == -1) return dfa;
    
    Subset* subs = malloc(MAX_SUBSETS * sizeof(Subset));
    for (i = 0; i < MAX_SUBSETS; i++) subs[i].states = malloc(nfa->num_states * sizeof(int));
    
    subs[0].count = 1;
    subs[0].states[0] = init;
    make_name(&subs[0], nfa);
    int n = 1, p = 0;
    
    add_state(dfa, subs[0].name);
    set_initial_state(dfa, subs[0].name);
    if (is_final(nfa, &subs[0])) add_final_state(dfa, subs[0].name);
    
    while (p < n) {
        for (int sym = 0; sym < nfa->num_symbols; sym++) {
            Subset res;
            res.states = malloc(nfa->num_states * sizeof(int));
            move(nfa, &subs[p], nfa->alphabet[sym].symbol, &res);
            
            if (res.count) {
                int found = -1;
                for (i = 0; i < n; i++) if (equals(&res, &subs[i])) { found = i; break; }
                
                if (found == -1 && n < MAX_SUBSETS) {
                    subs[n].count = res.count;
                    for (i = 0; i < res.count; i++) subs[n].states[i] = res.states[i];
                    strcpy(subs[n].name, res.name);
                    add_state(dfa, subs[n].name);
                    if (is_final(nfa, &subs[n])) add_final_state(dfa, subs[n].name);
                    found = n++;
                }
                if (found != -1) add_transition(dfa, subs[p].name, nfa->alphabet[sym].symbol, subs[found].name);
            }
            free(res.states);
        }
        p++;
    }
    
    for (i = 0; i < MAX_SUBSETS; i++) free(subs[i].states);
    free(subs);
    return dfa;
}
