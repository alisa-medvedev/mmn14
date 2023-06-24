#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_table.h"

#define INITIAL_CAPACITY 10


void add_line_to_macro(struct macro *macro, const char *line) {
    // Resize the lines array within the macro
    macro->lines = realloc(macro->lines, (macro->num_of_lines + 1) * sizeof(char *));

    // Add the new line to the macro
    macro->lines[macro->num_of_lines] = strdup(line);
    macro->num_of_lines++;
}

void initialize_macro_table(struct macro_table *table) {
    table->macros = malloc(INITIAL_CAPACITY * sizeof(struct macro));
    table->num_macros = 0;
    table->capacity = INITIAL_CAPACITY;
}

void add_macro_to_table(struct macro_table *table, const struct macro *new_macro) {
    // Resize the macro table if it's full
    if (table->num_macros == table->capacity) {
        table->capacity *= 2;
        table->macros = realloc(table->macros, table->capacity * sizeof(struct macro));
    }

    // Add the new macro to the macro table
    table->macros[table->num_macros] = *new_macro;
    table->num_macros++;
}


void free_macro_table(struct macro_table *table) {
    for (int i = 0; i < table->num_macros; i++) {
        for (int j = 0; j < table->macros[i].num_of_lines; j++) {
            free(table->macros[i].lines[j]);
        }
        free(table->macros[i].lines);
    }
    free(table->macros);
    table->num_macros = 0;
    table->capacity = 0;
}
