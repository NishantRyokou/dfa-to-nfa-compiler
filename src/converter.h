#ifndef CONVERTER_H
#define CONVERTER_H

#include "dfa.h"
#include "nfa.h"

NFA* convert_dfa_to_nfa(DFA* dfa);

#endif
