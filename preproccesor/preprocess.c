#include <stdio.h>
#include <string.h>
#include "macro_table.h"

#define MAX_MACRO_NAME_LEN 31
#define MAX_LINE_LENGTH 90

enum type_of_line{
    macro_definition,
    comment,
    macro_call,
    not_a_macro,
    null_line
};

void skip_spaces(char* str) { 
    while (*str == ' ') {
        str++;
    }
}

enum type_of_line check_as_line_type(struct macro_table *macro_table, char *line ) {   
    char *token;
    if(*line == '\0')
        return null_line;
    if(*line == ';')
        return comment;
    skip_spaces(line);

    if (strstr(line,"mcro") != NULL){
        return(macro_definition);
    }
    if (search_macro_name(macro_table,line) != -1)
    {
        return macro_call;
    }
    return not_a_macro;
}

const char *open_macros(char *file_name){
    char as_file_name[strlen(file_name) + strlen(".as") + 1];
    char am_file_name[strlen(file_name) + strlen(".am") + 1];
    FILE *am_file;
    FILE *as_file;
    char line_buffer[MAX_LINE_LENGTH];
    int macro_index;
    int i;

    strcat(strcpy(as_file_name,file_name),".as");
    strcat(strcpy(am_file_name,file_name),".am");

    as_file = fopen(as_file_name,"r");
    if (as_file == NULL)
    {
        return NULL;
    }
    am_file = fopen(am_file_name,"w");
    if (as_file == NULL)
    {
        return NULL;
    }

    struct macro_table table;
    initialize_macro_table(&table);
    struct macro temp_macro;
    initialize_macro(&temp_macro);
    char *temp_macro_name;
    
    while(fgets(line_buffer,MAX_LINE_LENGTH,as_file))
    {
        switch (check_as_line_type(&table,line_buffer))
        {
        case macro_definition:
        if (temp_macro_name != NULL) {
                // Reset temp_macro and temp_macro_name for the next iteration
                initialize_macro(&temp_macro);
                temp_macro_name = NULL;
            }
            temp_macro_name = *line_buffer += 4;
            skip_spaces(temp_macro_name);
            strcpy(temp_macro.macro_name, temp_macro_name);
            while(fgets(line_buffer,MAX_LINE_LENGTH,as_file)){
                if (strstr(line_buffer,"endmcro" != NULL)){
                    break;
                }
                add_line_to_macro(&temp_macro, temp_macro_name);
            }
            add_macro_to_table(&table,&temp_macro);
            break;

        case macro_call:
            temp_macro_name = *line_buffer;
            skip_spaces(temp_macro_name);
            macro_index = search_macro_name(&table,line_buffer);
            for ( i = 0; i < table.macros[macro_index].num_of_lines; i++)
            {
                fputs(table.macros[macro_index].lines[i],am_file);
            }
            break;
        case not_a_macro:
            break;
        case comment:
            break;
        case null_line:
            break;
        default:
            break;
        }
    }
    free_macro_table(&table);
}