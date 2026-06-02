%{
#include <string.h>
#include "dfa.h"
#include "parser.tab.h"

int line_num = 1;
%}

%option noyywrap

DIGIT       [0-9]
LETTER      [a-zA-Z_]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*

%%

"DFA"                   { return DFA_KW; }
"NFA"                   { return NFA_KW; }
"states"                { return STATES_KW; }
"alphabet"              { return ALPHABET_KW; }
"initial"               { return INITIAL_KW; }
"finals"                { return FINALS_KW; }
"transitions"           { return TRANSITIONS_KW; }

\{                      { return LBRACE; }
\}                      { return RBRACE; }
:                       { return COLON; }
,                       { return COMMA; }
"--"                    { return DASHARROW; }
"->"                    { return ARROW; }

[a-z0-9]                {
                            yylval.c = yytext[0];
                            return SYMBOL;
                        }

{IDENTIFIER}            { 
                            yylval.str = strdup(yytext); 
                            return ID; 
                        }

[ \t\r]+                { }
\n                      { line_num++; }

.                       { 
                            fprintf(stderr, "Unexpected character: %c at line %d\n", yytext[0], line_num);
                        }

%%
