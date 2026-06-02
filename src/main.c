#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dfa.h"
#include "nfa.h"
#include "converter.h"

extern DFA* current_dfa;
extern FILE* yyin;
extern int yyparse();

#define MAX_EXAMPLES 20
#define MAX_DFA_LENGTH 512
#define FILE_BUFFER_SIZE 8192

typedef struct {
    char* dfa_spec;  /* Dynamic allocation for flexibility */
} DFAExample;

DFAExample* examples = NULL;
int num_examples = 0;
int capacity_examples = 0;

int load_examples(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return 0;
    }
    
    /* Use buffered I/O for better performance */
    char buffer[FILE_BUFFER_SIZE];
    setvbuf(file, buffer, _IOFBF, FILE_BUFFER_SIZE);
    
    /* Read number of examples */
    if (fscanf(file, "%d", &num_examples) != 1) {
        fprintf(stderr, "Error: Could not read number of examples\n");
        fclose(file);
        return 0;
    }
    
    if (num_examples > MAX_EXAMPLES) {
        num_examples = MAX_EXAMPLES;
    }
    
    /* Pre-allocate example array */
    examples = (DFAExample*)malloc(num_examples * sizeof(DFAExample));
    if (!examples) {
        fprintf(stderr, "Error: Could not allocate memory for examples\n");
        fclose(file);
        return 0;
    }
    capacity_examples = num_examples;
    
    /* Read each example */
    fgetc(file); /* consume newline after number */
    char temp_spec[MAX_DFA_LENGTH];
    for (int i = 0; i < num_examples; i++) {
        if (fgets(temp_spec, MAX_DFA_LENGTH, file) == NULL) {
            fprintf(stderr, "Error: Could not read example %d\n", i + 1);
            fclose(file);
            return i;
        }
        
        /* Remove trailing newline */
        size_t len = strlen(temp_spec);
        if (len > 0 && temp_spec[len - 1] == '\n') {
            temp_spec[len - 1] = '\0';
            len--;
        }
        
        /* Allocate exact size needed for this example */
        examples[i].dfa_spec = (char*)malloc(len + 1);
        if (!examples[i].dfa_spec) {
            fprintf(stderr, "Error: Could not allocate memory for example %d\n", i + 1);
            fclose(file);
            return i;
        }
        strcpy(examples[i].dfa_spec, temp_spec);
    }
    
    fclose(file);
    return num_examples;
}

void cleanup_examples() {
    if (examples) {
        for (int i = 0; i < num_examples; i++) {
            free(examples[i].dfa_spec);
        }
        free(examples);
        examples = NULL;
    }
}

void display_examples() {
    printf("\n========================================\n");
    printf("Available DFA Examples:\n");
    printf("========================================\n\n");
    
    for (int i = 0; i < num_examples; i++) {
        printf("[%d] %s\n\n", i + 1, examples[i].dfa_spec);
    }
}

int main(int argc, char* argv[]) {
    printf("╔════════════════════════════════════════╗\n");
    printf("║      DFA to NFA Converter              ║\n");
    printf("║      Using Flex and Bison              ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
    
    /* Load examples from file */
    const char* examples_file = "tests/dfa_examples.txt";
    int loaded = load_examples(examples_file);
    
    if (loaded == 0) {
        fprintf(stderr, "No examples loaded. Make sure %s exists.\n", examples_file);
        return 1;
    }
    
    printf("Successfully loaded %d DFA examples.\n", loaded);
    
    /* Display examples */
    display_examples();
    
    /* Get user selection */
    int choice;
    while (1) {
        printf("Enter the number of the DFA example to convert (1-%d): ", num_examples);
        fflush(stdout);
        
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); /* Clear input buffer */
            continue;
        }
        
        if (choice < 1 || choice > num_examples) {
            fprintf(stderr, "Invalid choice. Please enter a number between 1 and %d.\n", num_examples);
            continue;
        }
        
        break;
    }
    
    printf("\n========================================\n");
    printf("Converting Example %d\n", choice);
    printf("========================================\n\n");
    
    /* Create DFA for conversion */
    current_dfa = create_dfa();
    
    /* Create temporary file for parsing - use binary mode for efficiency */
    FILE* temp_file = fopen("dfa_input.tmp", "w");
    if (!temp_file) {
        fprintf(stderr, "Error: Could not create temporary file\n");
        free_dfa(current_dfa);
        cleanup_examples();
        return 1;
    }
    
    /* Write selected DFA spec to temp file */
    fprintf(temp_file, "%s\n", examples[choice - 1].dfa_spec);
    fclose(temp_file);
    
    /* Read from temp file with buffering */
    FILE* read_file = fopen("dfa_input.tmp", "r");
    if (!read_file) {
        fprintf(stderr, "Error: Could not read temporary file\n");
        free_dfa(current_dfa);
        cleanup_examples();
        return 1;
    }
    
    /* Set up buffered reading */
    char file_buffer[FILE_BUFFER_SIZE];
    setvbuf(read_file, file_buffer, _IOFBF, FILE_BUFFER_SIZE);
    
    yyin = read_file;
    
    if (yyparse() != 0) {
        fprintf(stderr, "Parsing failed\n");
        fclose(read_file);
        free_dfa(current_dfa);
        cleanup_examples();
        remove("dfa_input.tmp");
        return 1;
    }
    
    fclose(read_file);
    
    printf("\n>>> INPUT DFA <<<\n");
    print_dfa(current_dfa);
    
    printf("\n\n>>> CONVERTED NFA <<<\n");
    NFA* nfa = convert_dfa_to_nfa(current_dfa);
    print_nfa(nfa);
    
    printf("\n========================================\n");
    printf("Conversion Complete!\n");
    printf("========================================\n\n");
    
    /* Cleanup */
    free_dfa(current_dfa);
    free_nfa(nfa);
    cleanup_examples();
    
    /* Remove temporary file */
    remove("dfa_input.tmp");
    
    return 0;
}
