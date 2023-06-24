#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

#define MAX_MACRO_NAME_LEN 50

struct macro {
    char macro_name[MAX_MACRO_NAME_LEN + 1];
    int num_of_lines;
    char **lines;
};

struct macro_table {
    struct macro *macros;
    int num_macros;
    int capacity;
};

void add_line_to_macro(struct macro *macro, const char *line);
void initialize_macro_table(struct macro_table *table);
void add_macro_to_table(struct macro_table *table, const struct macro *new_macro);
void free_macro_table(struct macro_table *table);

#endif /* MACRO_TABLE_H */