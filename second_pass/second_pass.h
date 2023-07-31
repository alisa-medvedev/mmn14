#ifndef SECOND_PASS_H
#define SECOND_PASS_H


/* Function prototype for fpass_line */
void spass_line(struct object_file *obj,struct entrie** entries, struct external** externals, int *IC, int *DC, table *symbol_table, struct syntex_tree *ast);

void create_obj_file(struct object_file *obj, FILE **ob_file_ptr, char* file_name);

void create_ent_file(struct entrie** entries,FILE **ent_file_ptr, char* file_name);

void create_ext_file(struct external** externals,FILE **ext_file_ptr, char* file_name);

#endif