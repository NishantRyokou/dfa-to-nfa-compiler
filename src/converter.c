#include "converter.h"

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
