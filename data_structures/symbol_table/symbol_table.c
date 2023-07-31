#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

/* function to add a new symbol to the symbol table */
void add_table_item(table* sym_table, char* sym_name, int sym_address, enum symbol_type type) {
    /* freate a new symbol and initialize its values */
    struct symbol* new_symbol = (struct symbol*)malloc(sizeof(struct symbol));
    new_symbol->next = NULL;
    new_symbol->address = sym_address;
    new_symbol->sym_name = (char*)malloc(strlen(sym_name) + 1); 
    strcpy(new_symbol->sym_name, sym_name); 
    new_symbol->type = type;

    /* add the symbol to the table */
    if (*sym_table == NULL) {
        *sym_table = new_symbol;
    } else {
        struct symbol* current = *sym_table;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_symbol;
    }
}

/* function to free the memory allocated for the symbol table */
void free_table(table* sym_table) {
    struct symbol* current = *sym_table;
    while (current != NULL) {
        struct symbol* next = current->next;
        free(current->sym_name);
        free(current);
        current = next;
    }
    *sym_table = NULL;
}

/* function to get the address of a symbol by its name */
int get_address_by_name(table* sym_table, char* sym_name) {
    struct symbol* current = *sym_table;
    while (current != NULL) {
        if (strcmp(current->sym_name, sym_name) == 0) {
            return current->address;
        }
        current = current->next;
    }
    return -1; 
}

/* function to get a symbol node by its name */
struct symbol* get_symbol_by_name(table* sym_table, char* sym_name) {
    struct symbol* current = (*sym_table);
    while (current != NULL) {
        if (strcmp(current->sym_name, sym_name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; 
}

/* function to add the instruction counter (IC) to the addresses of data symbols */
void add_IC_to_data_sym(table* sym_table, int IC) {
    struct symbol* current = *sym_table;
    while (current != NULL) {
        /* Check if the symbol is a data symbol (including entry_string_symbol) */
        if (current->type >= data_symbol && current->type <= entry_string_symbol) {
            current->address += IC; 
        }
        current = current->next;
    }
}