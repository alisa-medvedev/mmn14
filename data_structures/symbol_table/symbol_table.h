#ifndef _TABLE_H
#define _TABLE_H

typedef struct symbol* table;

enum symbol_type {
	entry_symbol,
	external_symbol,
	code_symbol,
	data_symbol,
	string_symbol,
	entry_data_symbol,
	entry_string_symbol,
	entry_code_symbol
};

struct symbol {
	table next;
	int address;
	char* sym_name;
	enum symbol_type type;
};

void add_table_item(table* sym_table, char* sym_name, int adress, enum symbol_type type);

void free_table(table* sym_table);

int get_address_by_name(table* sym_table, char* sym_name);

struct symbol* get_symbol_by_name(table* sym_table, char* sym_name);

void add_IC_to_data_sym(table* sym_table, int IC);

#endif
