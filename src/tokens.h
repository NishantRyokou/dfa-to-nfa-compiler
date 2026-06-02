#ifndef TOKENS_H
#define TOKENS_H

/* Token types for lexer */
#define DFA_KW 1
#define STATES_KW 2
#define ALPHABET_KW 3
#define INITIAL_KW 4
#define FINALS_KW 5
#define TRANSITIONS_KW 6
#define LBRACE 7
#define RBRACE 8
#define COLON 9
#define COMMA 10
#define DASHARROW 11
#define ARROW 12
#define ID 13
#define SYMBOL 14

typedef union {
    char* str;
    char c;
    int num;
} YYSTYPE;

extern YYSTYPE yylval;

#endif
