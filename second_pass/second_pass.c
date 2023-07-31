#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer/lexer.h"
#include "data_structures/symbol_table/symbol_table.h"
#include "data_structures/object_file/object_file.h"
#include "second_pass/second_pass.h"
#define MAX_LINE_LENGTH 90

#define BASE_64 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
#define IC_ORG 100
#define NUM_OF_NUMS 10

static const char base64_table[] = BASE_64;

void spass_line(struct object_file *obj,struct entrie** entries, struct external** externals, int *IC, int *DC, table *symbol_table, struct syntex_tree *ast){
    struct symbol *temp_sym = NULL;
    int machine_word = 0;
    int instruction = 0;
    int i;

     if (ast->label[0] != '\0'){
        /* if the line is data/string/code and the symbol was defined as entry , add him to entries */
            temp_sym = get_symbol_by_name(symbol_table, ast->label);
            if (ast->line_type == inst_line && temp_sym->type == entry_code_symbol){
                add_entry(entries, temp_sym->sym_name, temp_sym->address);
            }
            else if (ast->line_type == dir_line){
                switch (ast->inst_or_dir.dir_line.dir_option)
                  {
                   case dir_data_line: case dir_string_line:
                        if (temp_sym->type == entry_data_symbol || temp_sym->type == entry_string_symbol){   
                            add_entry(entries,temp_sym->sym_name,temp_sym->address);
                        }
                   break;
                  default:
                    break;
                  }
            }

    }
            
        
    /*go over the insructions and fill the missing labels and externals*/
    switch (ast->line_type){         
        case inst_line:
            (*IC)++;
            instruction = (int)ast->inst_or_dir.inst_line.inst_name;
            if (instruction >= mov && instruction <= jsr)
            {
                if (ast->inst_or_dir.inst_line.inst_opperands_option[0] == register_op && ast->inst_or_dir.inst_line.inst_opperands_option[1] == register_op)
                { 
                    (*IC)++;
                }else{
                    for( i = 0; i < 2; i++)
                    {
                        switch (ast->inst_or_dir.inst_line.inst_opperands_option[i])
                        {
                        case label_op:
                            temp_sym = get_symbol_by_name(symbol_table, ast->inst_or_dir.inst_line.inst_opperands[i].label);
                            if (temp_sym != NULL){
                                /*the symbol was defined as external and now being called , add him to externals 
                                and add 000000000001 word*/
                                if (temp_sym->type == external_symbol){
                                add_external(externals,temp_sym->sym_name,*IC);
                                machine_word |= 1; 
                            }else{        
                                /*add label addres with 10 ending*/                    
                                machine_word = temp_sym->address;
                                machine_word = machine_word << 2;
                                machine_word |= 2;
                             }                               
                            }
                            obj->code_img[*IC-IC_ORG] = machine_word;                
                            break;
                        case no_op:
                            break;
                        default:
                            break;
                        }
                        /*if empty opperant dont add to the code_img*/
                        if (ast->inst_or_dir.inst_line.inst_opperands_option[i] != no_op) {
                            (*IC)++;
                        }
                        
                    }
                    
                }
                
            }
            break;
        default:
            break;
        }
}

/*functiuon that takes an int as parameter and encodes it to base64*/
char *base64_encode(int n) {
    int masked_value;
    int base64_index_1;
    int base64_index_2;
    static char result[3];
    /*extract the 12-bit value from the least significant bits*/
    masked_value = n & 0xFFF;

    /* extract the 6 most significant bits*/
    base64_index_1 = (masked_value >> 6) & 0x3F;

    /* extract the 6 least significant bits*/
    base64_index_2 = masked_value & 0x3F;

    result[0] = base64_table[base64_index_1];
    result[1] = base64_table[base64_index_2];
    result[2] = '\0';

    return result;
}

/*function that creates .ob file and puts there the data collected*/
void create_obj_file(struct object_file *obj, FILE **ob_file_ptr, char* file_name){
    char* ob_file_name;
    int i;
    char* base64_str;
    char combined_str[NUM_OF_NUMS];
    /* create a string containing the code length and data length */
    sprintf(combined_str, "%d %d\n", obj->code_len, obj->data_len);

    ob_file_name = malloc(strlen(file_name) + strlen(".ob") + 1);
    if (ob_file_name == NULL) {
    }
    strcat(strcpy(ob_file_name, file_name), ".ob");
    *ob_file_ptr = fopen(ob_file_name, "w");
    if (*ob_file_ptr == NULL) {
        printf("Failed to open .ob file");
    }
    /* write the code and data lengths to the .ob file */
    fputs(combined_str, *ob_file_ptr);

    /* write the code image in base64 format to the .ob file */
    for (i = 0; i < obj->code_len ; i++){
        base64_str = base64_encode(obj->code_img[i]);
        fputs(base64_str, *ob_file_ptr);
        fputs("\n", *ob_file_ptr);
    }

    /* write the data image in base64 format to the .ob file */
    for (i = 0; i < obj->data_len ; i++){
        base64_str = base64_encode(obj->data_img[i]);
        fputs(base64_str, *ob_file_ptr);
        fputs("\n", *ob_file_ptr);
    }

    free(ob_file_name);
    fclose(*ob_file_ptr);
}

/*function that creates the .ent file*/
void create_ent_file(struct entrie** entries, FILE **ent_file_ptr, char* file_name){
    char *ent_file_name;
    struct entrie* current = *entries;
    /*if no entries dont procceed*/
    if (current == NULL){
        return;
    }

    ent_file_name = malloc(strlen(file_name) + strlen(".ent") + 1);
    if (ent_file_name == NULL) {
    }
    strcat(strcpy(ent_file_name, file_name), ".ent");
    *ent_file_ptr = fopen(ent_file_name, "w");
    if (*ent_file_ptr == NULL) {
        printf("Failed to open .ent file");
    }

    /* write the entries to the .ent file */
    while (current != NULL) {
        char entry_string[MAX_LINE_LENGTH];
        sprintf(entry_string, "%-10s %d\n", current->label, current->address);
        fputs(entry_string, *ent_file_ptr);
        current = current->next;
    }
    
    free(ent_file_name);
    fclose(*ent_file_ptr);
}

/*function that creates the .ent file*/
void create_ext_file(struct external** externals, FILE **ext_file_ptr, char* file_name){
    char *ext_file_name;
    struct external* current = *externals;
    if (current == NULL){
        return;
    }

    ext_file_name = malloc(strlen(file_name) + strlen(".ext") + 1);
    if (ext_file_name == NULL) {
    }
    strcat(strcpy(ext_file_name, file_name), ".ext");

    *ext_file_ptr = fopen(ext_file_name, "w");
    if (*ext_file_ptr == NULL) {
        printf("Failed to open .ext file");
    }

    /* write the externals to the .ext file */
    while (current != NULL) {
        char entry_string[MAX_LINE_LENGTH];
        sprintf(entry_string, "%-10s %d\n", current->label, current->address);
        fputs(entry_string, *ext_file_ptr);
        current = current->next;
    }
    
    free(ext_file_name);
    fclose(*ext_file_ptr);
}