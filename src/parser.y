%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfa.h"
#include "converter.h"

extern int yylex();
extern int line_num;

void yyerror(const char* msg);

DFA* current_dfa = NULL;

int yylex_destroy(void);
%}

%union {
    char* str;
    char c;
    int num;
}

%token DFA_KW STATES_KW ALPHABET_KW INITIAL_KW FINALS_KW TRANSITIONS_KW
%token LBRACE RBRACE COLON COMMA DASHARROW ARROW
%token <str> ID
%token <c> SYMBOL

%start program

%%

program:
    DFA_KW LBRACE dfa_body RBRACE
    {
        printf("DFA parsed successfully\n");
    }
    | DFA_KW LBRACE RBRACE
    {
        printf("DFA parsed successfully (empty)\n");
    }
    ;

dfa_body:
    dfa_statement
    | dfa_body dfa_statement
    ;

dfa_statement:
    STATES_KW COLON state_list
    | ALPHABET_KW COLON symbol_list
    | INITIAL_KW COLON ID
    {
        if (current_dfa) {
            set_initial_state(current_dfa, $3);
            free($3);
        }
    }
    | FINALS_KW COLON final_state_list
    | TRANSITIONS_KW COLON transition_list
    ;

state_list:
    ID
    {
        if (current_dfa) {
            add_state(current_dfa, $1);
            free($1);
        }
    }
    | state_list COMMA ID
    {
        if (current_dfa) {
            add_state(current_dfa, $3);
            free($3);
        }
    }
    ;

symbol_list:
    SYMBOL
    {
        if (current_dfa) {
            add_symbol(current_dfa, $1);
        }
    }
    | symbol_list COMMA SYMBOL
    {
        if (current_dfa) {
            add_symbol(current_dfa, $3);
        }
    }
    ;

final_state_list:
    ID
    {
        if (current_dfa) {
            add_final_state(current_dfa, $1);
            free($1);
        }
    }
    | final_state_list COMMA ID
    {
        if (current_dfa) {
            add_final_state(current_dfa, $3);
            free($3);
        }
    }
    ;

transition_list:
    transition
    | transition_list transition
    ;

transition:
    ID DASHARROW SYMBOL ARROW ID
    {
        if (current_dfa) {
            add_transition(current_dfa, $1, $3, $5);
            free($1);
            free($5);
        }
    }
    ;

%%

void yyerror(const char* msg) {
    fprintf(stderr, "Error at line %d: %s\n", line_num, msg);
}
