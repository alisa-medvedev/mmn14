#include <stdio.h>
#include <string.h>
#include "macro_table.h"

#define MAX_MACRO_NAME_LEN 31
#define MAX_LINE_LENGTH 90

enum type_of_line{
    macro_definition,
    macro_call,
    not_a_macro,
    null_line
};

enum type_of_line check_as_line_type(struct macro_table *macro_table, char *line ) {
    
}

const char *open_macros(char *file_name){
    char as_file_name[strlen(file_name) + strlen(".as") + 1];
    char am_file_name[strlen(file_name) + strlen(".am") + 1];
    FILE *am_file;
    FILE *as_file;
    char line_buffer[MAX_LINE_LENGTH];
    enum type_of_line line_type;

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
    
    while(fgets(line_buffer,MAX_LINE_LENGTH,as_file))
    {
        switch (check_as_line_type(&table,line_buffer))
        {
        case macro_definition:
            /* code */
            break;
        case macro_call:
            /* code */
            break;
        case not_a_macro:
            /* code */
            break;
        case null_line:
            /* code */
            break;
        default:
            break;
        }
    }
    



    

}