#ifndef CONVERTER_H
#define CONVERTER_H

#include "dfa.h"
#include "nfa.h"

/* DFA to NFA conversion (simple) */
NFA* convert_dfa_to_nfa(DFA* dfa);

/* NFA to DFA conversion (subset construction / powerset algorithm) */
DFA* convert_nfa_to_dfa(NFA* nfa);

#endif
