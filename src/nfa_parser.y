%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nfa.h"

extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);

NFA* current_nfa = NULL;
%}

%union {
    char* string;
    char character;
}

%token <string> IDENTIFIER
%token <character> SYMBOL
%token NFA STATES ALPHABET INITIAL FINALS TRANSITIONS
%token LBRACE RBRACE COMMA ARROW DASH

%start nfa_spec

%%

nfa_spec:
    NFA LBRACE nfa_components RBRACE
    ;

nfa_components:
    states_section alphabet_section initial_section finals_section
    | states_section alphabet_section initial_section finals_section transitions_section
    ;

states_section:
    STATES ':' state_list
    ;

state_list:
    IDENTIFIER { add_nfa_state(current_nfa, $1); free($1); }
    | state_list COMMA IDENTIFIER { add_nfa_state(current_nfa, $3); free($3); }
    ;

alphabet_section:
    ALPHABET ':' symbol_list
    ;

symbol_list:
    SYMBOL { add_nfa_symbol(current_nfa, $1); }
    | symbol_list COMMA SYMBOL { add_nfa_symbol(current_nfa, $3); }
    ;

initial_section:
    INITIAL ':' IDENTIFIER { set_nfa_initial_state(current_nfa, $3); free($3); }
    ;

finals_section:
    FINALS ':' final_list
    ;

final_list:
    IDENTIFIER { add_nfa_final_state(current_nfa, $1); free($1); }
    | final_list COMMA IDENTIFIER { add_nfa_final_state(current_nfa, $3); free($3); }
    ;

transitions_section:
    TRANSITIONS ':' transition_list
    ;

transition_list:
    transition
    | transition_list transition
    ;

transition:
    IDENTIFIER DASH DASH SYMBOL DASH '>' IDENTIFIER {
        add_nfa_transition(current_nfa, $1, $4, $7);
        free($1);
        free($7);
    }
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
}
