#include <stdio.h>
#include <string.h>

#define MAX_MACRO_NAME_LEN 31
#define MAX_LINE_LENGTH 81

struct macro
{
    char macro_name[MAX_MACRO_NAME_LEN+1];
    int num_of_lines;
    char **lines;
};

enum type_of_line{
    macro_definition,
    macro_call,
    not_a_macro,
    null_line
};

enum type_of_line check_as_file_line() {
    
}

const char *open_macros(char *file_name){
    char as_file_name[strlen(file_name) + strlen(".as") + 1];
    char am_file_name[strlen(file_name) + strlen(".am") + 1];
    FILE *am_file;
    FILE *as_file;
    char line_buffer[MAX_LINE_LENGTH];

    strcat(strcpy(as_file_name,file_name),".as");
    strcat(strcpy(as_file_name,file_name),".am");

    as_file = fopen(as_file_name,"r");
    if (as_file == NULL)
    {
        printf("Failed to open the as file.\n");
    }
    am_file = fopen(am_file_name,"r");
    if (as_file == NULL)
    {
        printf("Failed to open the am file.\n");
    }

    

}