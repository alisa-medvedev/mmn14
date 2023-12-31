#ifndef MACRO_TABLE_H
#define MACRO_TABLE_H

#define MAX_MACRO_NAME_LEN 50
#define INITIAL_CAPACITY 10
#define INCREASE_LINES 5
#define MAX_LINE_LENGTH 90

struct macro {
    char macro_name[MAX_MACRO_NAME_LEN + 1];
    int num_of_lines;
    int capacity;
    char **lines;
};

struct macro_table {
    struct macro *macros;
    int num_macros;
    int capacity;
};

void add_line_to_macro(struct macro *macro, const char *line);
void initialize_macro(struct macro *macro);
void initialize_macro_table(struct macro_table *table);
void add_macro_to_table(struct macro_table *table, const struct macro *new_macro);
int search_macro_name(const struct macro_table *table, const char *name);
void free_macro(struct macro *macro);
void free_macro_table(struct macro_table *table);
int compare_until_whitespace(const char *str1, const char *str2);

#endif /* MACRO_TABLE_H */