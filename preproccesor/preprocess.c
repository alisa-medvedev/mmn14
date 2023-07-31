#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "macro_table.h"

#define MAX_LINE_LENGTH 90
#define SPACE_CHARS "\n\t "

/* enum to represent different types of lines */
enum type_of_line{
    macro_definition,
    comment,
    macro_call,
    not_a_macro,
    null_line
};

/* function to duplicate a string */
char *strdup(const char *str) {
    size_t length = strlen(str);
    char *duplicate = (char *)malloc((length + 1) * sizeof(char));
    if (duplicate != NULL) {
        strcpy(duplicate, str);
    }
    return duplicate;
}

/* function to skip leading spaces in a string */
void skip_spaces(char *line) {
    int i = 0;
    int j = 0;

    while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r') {
        i++;
    }
    while (line[i] != '\0') {
        line[j] = line[i];
        i++;
        j++;
    }

    line[j] = '\0';
}

/* function that determines the type of line */
enum type_of_line check_line_type(struct macro_table *macro_table, char *line ) {   
    if(*line == '\0')
        return null_line;
    if(*line == ';')
        return comment;

    if (strncmp(line, "mcro", 4) == 0){
        return(macro_definition);
    }
    if (search_macro_name(macro_table, line) != -1)
    {
        return macro_call;
    }
    return not_a_macro;
}

/* function to open the input and output files and process the macros */
void open_macros(char *file_name, FILE **am_file_ptr){
    char* as_file_name;
    char* am_file_name;
    struct macro_table table;
    struct macro temp_macro;
    char *temp_macro_name;
    char line_buffer[MAX_LINE_LENGTH];
    int macro_index;
    int i;
    char *line;
    FILE* as_file;
    
    /* open as file and add .as extension */
    as_file_name = malloc(strlen(file_name) + strlen(".as") + 1);
    if (as_file_name == NULL) {
        printf("Memory allocation error");
        exit(1);
    }
    strcat(strcpy(as_file_name, file_name), ".as");
    as_file = fopen(as_file_name, "r");
    if (as_file == NULL) {
        printf("Failed to open .as file");
        exit(1);
    }

    /* open am file and add .am extension */
    am_file_name = malloc(strlen(file_name) + strlen(".am") + 1);
    if (am_file_name == NULL) {
    }
    strcat(strcpy(am_file_name, file_name), ".am");
    *am_file_ptr = fopen(am_file_name, "w");
    if (*am_file_ptr == NULL) {
        printf("Failed to open .am file");
    }

    initialize_macro_table(&table);
    initialize_macro(&temp_macro);
    
    /* process line by line from the input file */
    while(fgets(line_buffer,MAX_LINE_LENGTH,as_file ))
    {
        line = strdup(line_buffer);
        skip_spaces(line);
        switch (check_line_type(&table, line))
        {
        case macro_definition:
            line += 4;
            temp_macro_name = strdup(line);
            skip_spaces(temp_macro_name);
            line -= 4;

            strcpy(temp_macro.macro_name, temp_macro_name);
            free(temp_macro_name);
            while (fgets(line_buffer, MAX_LINE_LENGTH, as_file)) {
                free(line); 
                line = strdup(line_buffer);
                skip_spaces(line);
                if (strstr(line, "endmcro"))
                    break;
                
                add_line_to_macro(&temp_macro, line);
            }
            add_macro_to_table(&table, &temp_macro);
            
            free_macro(&temp_macro);
            break;

        case macro_call:
            macro_index = search_macro_name(&table, line);
            for (i = 0; i < table.macros[macro_index].num_of_lines; i++)
            {
                fputs(table.macros[macro_index].lines[i], *am_file_ptr);
            }
            line = NULL;
            break;
        case not_a_macro:
            fputs(line, *am_file_ptr);
            break;
        case comment:
            break;
        case null_line:
            break;
        default:
            break;
        }
        free(line);
    }
    free(as_file_name);
    fclose(as_file);
    free(am_file_name);
    fclose(*am_file_ptr);
    free_macro_table(&table);
}
