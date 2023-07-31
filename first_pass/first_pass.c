#include <stdio.h>
#include "lexer/lexer.h"
#include "data_structures/symbol_table/symbol_table.h"
#include "data_structures/object_file/object_file.h"
#include "first_pass/first_pass.h"

#define IC_ORG 100


void fpass_line(struct object_file *obj, int *IC, int *DC, table *symbol_table, struct syntex_tree *ast){
        
        struct symbol *temp_sym = NULL;
        unsigned int machine_word = 0;
        int instruction = 0;
        int i;
        /*label present*/
        if (ast->label[0] != '\0'){
            temp_sym = get_symbol_by_name(symbol_table, ast->label);
            if (ast->line_type == inst_line)
            {
               /*symbol exists, means that was defined as entry before*/
               if (temp_sym != NULL){
                    if (temp_sym->type != entry_symbol){
                        printf("error line %d: symbol redefenition - %s\n",*IC,temp_sym->sym_name );
                    }
                    else{
                        temp_sym->type = entry_code_symbol;
                        temp_sym->address = *IC;
                    }
               }
               /*symbol not exists*/
               else{
                add_table_item(symbol_table,ast->label,*IC,code_symbol);                 
               }
                    
            }
            else if (ast->line_type == dir_line)
            {                  
                  switch (ast->inst_or_dir.dir_line.dir_option)
                  {
                  case dir_data_line:
                  /*symbol exists, means that was defined as entry before*/
                    if (temp_sym != NULL){
                        if(temp_sym->type == entry_symbol){
                            temp_sym->type = entry_data_symbol;
                            temp_sym->address = *DC;
                        }
                    }else{
                        add_table_item(symbol_table, ast->label, *DC, data_symbol);
                    }              
                    break;

                  case dir_entry_line:
                  /*symbol exists, means that was defined as data or code symbol before*/
                     if (temp_sym != NULL){
                        temp_sym->type = (temp_sym->type == data_symbol) ? entry_data_symbol : entry_code_symbol;    
                    }else{
                        add_table_item(symbol_table, ast->label, 0, entry_symbol);
                    }
                    break;

                  case dir_extern_line:
                    add_table_item(symbol_table, ast->label, 0, external_symbol);
                    break;

                  case dir_string_line:
                  /*symbol exists, means that was defined as entry before*/
                    if (temp_sym != NULL){
                        if(temp_sym->type == entry_symbol){
                            temp_sym->type = entry_string_symbol;
                            temp_sym->address = *DC;
                        }
                    }else{
                        add_table_item(symbol_table, ast->label, *DC, string_symbol);
                    }
                    break;

                  default:
                    break;
                  }

            }
                            
        }
        

        switch (ast->line_type){         
        case dir_line:
            switch (ast->inst_or_dir.dir_line.dir_option)
            {
                /* handle .string directive */
                case dir_string_line:
                    i=0;
                    while (ast->inst_or_dir.dir_line.dir_info.str[i] != '\0') {
                        machine_word = (unsigned int)ast->inst_or_dir.dir_line.dir_info.str[i]; 
                        i++;
                        add_to_data_img(obj,machine_word,*DC);
                        (*DC)++;
                    }
                    /* Null-terminate the string with a zero word */
                    machine_word = 0;
                    add_to_data_img(obj,machine_word,*DC); 
                    (*DC)++;
                    break;
                /* Handle .data directive */
                case dir_data_line:
                    for ( i = 0; i < ast->inst_or_dir.dir_line.dir_info.num_array.num_arr_len ; i++){
                        machine_word = ast->inst_or_dir.dir_line.dir_info.num_array.num_arr[i];
                        add_to_data_img(obj,machine_word,*DC); 
                        (*DC)++;
                    }    
                    break;
                    
                default:
                    break;
            }
            break;

        case inst_line:
            /*set up the instruction machine word*/
            machine_word |= ast->inst_or_dir.inst_line.inst_opperands_option[0] << 9;
            machine_word |= ast->inst_or_dir.inst_line.inst_name << 5;
            machine_word |= ast->inst_or_dir.inst_line.inst_opperands_option[1] << 2;
            add_to_code_img(obj,machine_word,*IC); 
            (*IC)++;

            instruction = (int)ast->inst_or_dir.inst_line.inst_name;
            if (instruction >= mov && instruction <= jsr)
            {
                if (ast->inst_or_dir.inst_line.inst_opperands_option[0] == register_op && ast->inst_or_dir.inst_line.inst_opperands_option[1] == register_op)
                {
                    machine_word = ast->inst_or_dir.inst_line.inst_opperands[1].reg_num <<2;
                    machine_word |= ast->inst_or_dir.inst_line.inst_opperands[0].reg_num <<7;
                    add_to_code_img(obj,machine_word,*IC); 
                    (*IC)++;
                }else{
                    for( i = 0; i < 2; i++)
                    {
                        switch (ast->inst_or_dir.inst_line.inst_opperands_option[i])
                        {
                        case const_op:                        
                            machine_word = (unsigned int)ast->inst_or_dir.inst_line.inst_opperands[i].const_num << 2;
                            break;
                        case register_op:
                            machine_word = ast->inst_or_dir.inst_line.inst_opperands[i].reg_num << (7-(i*5));
                            break;
                        case label_op:
                            /*addres of the label is not known yet*/
                            machine_word = 0;                
                            break;
                        case no_op:
                            break;
                        default:
                            break;
                        }
                        /*if empty opperant dont add to the code_img*/
                        if (ast->inst_or_dir.inst_line.inst_opperands_option[i] != no_op) {
                            add_to_code_img(obj,machine_word,*IC);
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


