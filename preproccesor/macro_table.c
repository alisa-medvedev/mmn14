#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_table.h"

#define INITIAL_CAPACITY 10
#define INCREASE_LINES 5
#define MAX_LINE_LENGTH 90
#define MAX_MACRO_NAME_LENGTH 50



void add_line_to_macro(struct macro *macro, const char *line) {
    if (macro->num_of_lines >= macro->capacity) {
        macro->capacity *= 2;
        macro->lines = realloc(macro->lines, macro->capacity * sizeof(char*));
    }
    
    macro->lines[macro->num_of_lines] = malloc((MAX_LINE_LENGTH + 1) * sizeof(char));
    strcpy(macro->lines[macro->num_of_lines], line);
    
    macro->num_of_lines++;
}

void initialize_macro(struct macro *macro) {
    macro->capacity = INITIAL_CAPACITY;
    macro->num_of_lines = 0;
    macro->lines = malloc(macro->capacity * sizeof(char**));
}

void initialize_macro_table(struct macro_table *table) {
    table->macros = malloc(INITIAL_CAPACITY * sizeof(struct macro));
    table->num_macros = 0;
    table->capacity = INITIAL_CAPACITY;
}


void add_macro_to_table(struct macro_table *table, const struct macro *new_macro) {
    int new_capacity;
    struct macro *new_macros;
    struct macro *table_macro;
    int i;

    if (table->num_macros == table->capacity) {
        new_capacity = table->capacity * 2;
        new_macros = realloc(table->macros, new_capacity * sizeof(struct macro));
        if (new_macros == NULL) {
            printf("Error: Memory allocation failed when expanding macro table.\n");
            return;
        }
        table->macros = new_macros;
        table->capacity = new_capacity;
    }

    table_macro = &table->macros[table->num_macros];
    memcpy(table_macro, new_macro, sizeof(struct macro));

    table_macro->lines = malloc(table_macro->num_of_lines * sizeof(char*));
    for (i = 0; i < table_macro->num_of_lines; i++) {
        table_macro->lines[i] = malloc((MAX_LINE_LENGTH + 1) * sizeof(char));
        strcpy(table_macro->lines[i], new_macro->lines[i]);
    }

    table->num_macros++;
}


int search_macro_name(const struct macro_table *table, const char *name) {
    int i;
    for (i = 0; i < table->num_macros; i++) {
        if (compare_until_whitespace(table->macros[i].macro_name,name) == 0){
            return i;
        }
    }  
    return -1;
}

void free_macro(struct macro *macro) {
    int i;
    if (macro->lines != NULL) {
        for (i = 0; i < macro->num_of_lines; i++) {
            free(macro->lines[i]);
            macro->lines[i] = NULL; 
        }
    }
    macro->num_of_lines = 0; 
}

void free_macro_table(struct macro_table *table) {
    int i;
    int j;
    for (i = 0; i < table->num_macros; i++) {
        for (j = 0; j < table->macros[i].num_of_lines; j++) {
            free(table->macros[i].lines[j]);
        }
        free(table->macros[i].lines);
    }
    free(table->macros);
    table->num_macros = 0;
    table->capacity = 0;
}

int compare_until_whitespace(const char *str1, const char *str2) {
    int i = 0;    
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] == ' ' || str2[i] == ' ') 
            break;
        if (str1[i] != str2[i])
            return str1[i] - str2[i];
        i++;
    }
    return 0;
}

