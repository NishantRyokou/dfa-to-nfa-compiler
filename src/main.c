#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfa.h"
#include "nfa.h"
#include "converter.h"

extern DFA* current_dfa;
extern NFA* current_nfa;
extern FILE* yyin;
extern int yyparse();

#define MAX_EXAMPLES 20
#define MAX_LEN 512
#define BUF_SIZE 8192

typedef struct { char* spec; } Example;

Example* examples = NULL;
int num_examples = 0;

int load_examples(const char* file) {
    FILE* f = fopen(file, "r");
    if (!f) return 0;
    
    char buf[BUF_SIZE];
    setvbuf(f, buf, _IOFBF, BUF_SIZE);
    
    if (fscanf(f, "%d", &num_examples) != 1 || num_examples > MAX_EXAMPLES) {
        fclose(f); return 0;
    }
    
    examples = malloc(num_examples * sizeof(Example));
    if (!examples) { fclose(f); return 0; }
    
    fgetc(f);
    char temp[MAX_LEN];
    for (int i = 0; i < num_examples; i++) {
        if (!fgets(temp, MAX_LEN, f)) { fclose(f); return i; }
        size_t len = strlen(temp);
        if (len && temp[len-1] == '\n') temp[--len] = 0;
        examples[i].spec = malloc(len + 1);
        if (!examples[i].spec) { fclose(f); return i; }
        strcpy(examples[i].spec, temp);
    }
    fclose(f);
    return num_examples;
}

void cleanup() {
    if (examples) {
        for (int i = 0; i < num_examples; i++) free(examples[i].spec);
        free(examples);
    }
}

int get_input(const char* prompt, int max) {
    int val;
    while (1) {
        printf("%s", prompt);
        fflush(stdout);
        if (scanf("%d", &val) == 1 && val >= 1 && val <= max) return val;
        fprintf(stderr, "Invalid. Enter 1-%d.\n", max);
        while (getchar() != '\n');
    }
}

int main(int argc, char* argv[]) {
    printf("╔════════════════════════════════════════╗\n"
           "║   DFA ↔ NFA Bidirectional Converter   ║\n"
           "║      Using Flex and Bison              ║\n"
           "╚════════════════════════════════════════╝\n\n");
    
    int mode = get_input("Select Mode:\n  [1] DFA→NFA\n  [2] NFA→DFA\n\nChoice (1-2): ", 2);
    printf("\n");
    
    const char* file = (mode == 1) ? "tests/dfa_examples.txt" : "tests/nfa_examples.txt";
    if (!load_examples(file)) { fprintf(stderr, "Can't load %s\n", file); return 1; }
    
    printf("Loaded %d examples.\n\n", num_examples);
    for (int i = 0; i < num_examples; i++) printf("[%d] %s\n\n", i + 1, examples[i].spec);
    
    int choice = get_input("Select example: ", num_examples);
    printf("\n");
    
    const char* tmp = (mode == 1) ? "dfa.tmp" : "nfa.tmp";
    FILE* t = fopen(tmp, "w");
    if (!t) { cleanup(); return 1; }
    fprintf(t, "%s\n", examples[choice-1].spec);
    fclose(t);
    
    t = fopen(tmp, "r");
    if (!t) { cleanup(); return 1; }
    char buf[BUF_SIZE];
    setvbuf(t, buf, _IOFBF, BUF_SIZE);
    yyin = t;
    
    if (mode == 1) {
        current_dfa = create_dfa();
        if (yyparse()) { fclose(t); free_dfa(current_dfa); cleanup(); remove(tmp); return 1; }
        fclose(t);
        printf("\n>>> INPUT DFA <<<\n"); print_dfa(current_dfa);
        printf("\n>>> CONVERTED NFA <<<\n");
        NFA* nfa = convert_dfa_to_nfa(current_dfa);
        print_nfa(nfa);
        free_dfa(current_dfa);
        free_nfa(nfa);
    } else {
        current_nfa = create_nfa();
        if (yyparse()) { fclose(t); free_nfa(current_nfa); cleanup(); remove(tmp); return 1; }
        fclose(t);
        printf("\n>>> INPUT NFA <<<\n"); print_nfa(current_nfa);
        printf("\n>>> CONVERTED DFA <<<\n");
        DFA* dfa = convert_nfa_to_dfa(current_nfa);
        print_dfa(dfa);
        printf("\n--- Combined states show subset construction! ---\n");
        free_nfa(current_nfa);
        free_dfa(dfa);
    }
    
    printf("\nConversion Complete!\n");
    cleanup();
    remove(tmp);
    return 0;
}
