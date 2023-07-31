
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "preproccesor/macro_table.h"
#include "preproccesor/preprocess.h"
#include "lexer/lexer.h"
#include "data_structures/symbol_table/symbol_table.h"
#include "data_structures/object_file/object_file.h"
#include "first_pass/first_pass.h"
#include "second_pass/second_pass.h"

#define MAX_LINE_LEN 80
#define BASE_ADDRESS 100 
#define MAX_CODE_LEN 1000

static void compile_file(char *file_name);

struct syntex_tree ast_array[] = {
    {
        .syntax_error_msg = "",
        .label = "LENGTH",
        .line_type = dir_line,
        .inst_or_dir.dir_line = {
            .dir_option = dir_entry_line,
        }
    },
    {
        .syntax_error_msg = "",
        .label = "W",
        .line_type = dir_line,
        .inst_or_dir.dir_line = {
            .dir_option = dir_extern_line,
        }
    },
    {
        .syntax_error_msg = "",
        .label = "MAIN",
        .line_type = inst_line,
        .inst_or_dir.inst_line = {
            .inst_name = mov,
            .inst_opperands_option = {register_op, label_op},
            .inst_opperands = {{.reg_num = 3}, {.label = "LENGTH"}}
        }
    },
    {
        .syntax_error_msg = "",
        .label = "LOOP",
        .line_type = inst_line,
        .inst_or_dir.inst_line = {
            .inst_name = jmp,
            .inst_opperands_option = {no_op, label_op},
            .inst_opperands = {{}, {.label = "L1"}}
        }
    },
    {
        .syntax_error_msg = "",
        .label = "",
        .line_type = inst_line,
        .inst_or_dir.inst_line = {
            .inst_name = prn,
            .inst_opperands_option = {no_op, const_op},
            .inst_opperands = {{}, {.const_num = -5}}
        }
    },
    {
        .syntax_error_msg = "",
        .label = "",
        .line_type = inst_line,
        .inst_or_dir.inst_line = {
            .inst_name = bne,
            .inst_opperands_option = {no_op, label_op},
            .inst_opperands = {{}, {.label = "W"}}
        }
    },
    {
        .syntax_error_msg = "",
        .label = "",
        .line_type = inst_line,
        .inst_or_dir.inst_line = {
            .inst_name = sub,
            .inst_opperands_option = {register_op, register_op},
            .inst_opperands = {{.reg_num = 1}, {.reg_num = 4}}
        }
    },
    {
        .syntax_error_msg = "",
        .label = "",
        .line_type = inst_line,
        .inst_or_dir.inst_line = {
            .inst_name = bne,
            .inst_opperands_option = {no_op, label_op},
            .inst_opperands = {{}, {.label = "L3"}}
        }
    },
    {
        .syntax_error_msg = "",
        .label = "L1",
        .line_type = inst_line,
        .inst_or_dir.inst_line = {
            .inst_name = inc,
            .inst_opperands_option = {no_op, label_op},
            .inst_opperands = {{}, {.label = "K"}}
        }
    },
    {
        .syntax_error_msg = "",
        .label = "LOOP",
        .line_type = dir_line,
        .inst_or_dir.dir_line = {
            .dir_option = dir_entry_line,
        }
    },
    {
        .syntax_error_msg = "",
        .label = "",
        .line_type = inst_line,
        .inst_or_dir.inst_line = {
            .inst_name = jmp,
            .inst_opperands_option = {no_op, label_op,},
            .inst_opperands = {{}, {.label = "W"}}
        }
    },
    {
        .syntax_error_msg = "",
        .label = "END",
        .line_type = inst_line,
        .inst_or_dir.inst_line = {
            .inst_name = stop,
            .inst_opperands_option = {no_op, no_op},
            .inst_opperands = {{}, {}}
        }
    },
    {
        .syntax_error_msg = "",
        .label = "STR",
        .line_type = dir_line,
        .inst_or_dir.dir_line = {
            .dir_option = dir_string_line,
            .dir_info.str = "abcdef"
        }
    },
    {
        .syntax_error_msg = "",
        .label = "LENGTH",
        .line_type = dir_line,
        .inst_or_dir.dir_line = {
            .dir_option = dir_data_line,
            .dir_info.num_array = {6, -9, 15},
            .dir_info.num_array.num_arr_len = 3,
        }
    },
    {
        .syntax_error_msg = "",
        .label = "K",
        .line_type = dir_line,
        .inst_or_dir.dir_line = {
            .dir_option = dir_data_line,
            .dir_info.num_array = {22},
            .dir_info.num_array.num_arr_len = 1,
        }
    },
    {
        .syntax_error_msg = "",
        .label = "L3",
        .line_type = dir_line,
        .inst_or_dir.dir_line = {
            .dir_option = dir_extern_line,
        }
    }
};


static void compile_file(char *file_name) {
    FILE* am_file;
    FILE* ob_file;
    FILE* ext_file;
    FILE* ent_file;
    char* am_file_name;
    int i = 0;
    char line_buffer[MAX_LINE_LEN] = {0};
    struct syntex_tree ast;
    struct object_file obj = {0};
    struct entrie* entries = NULL;
    struct external* externals = NULL;
    table symbol_table = NULL;
    int IC = BASE_ADDRESS;
    int DC = 0;
     
   /* function that handles macro opening and preprocessor stuff.
      am_file will contain code ready for the first pass. */
    open_macros(file_name, &am_file);

    am_file_name = malloc(strlen(file_name) + strlen(".am") + 1);
    strcat(strcpy(am_file_name, file_name), ".am");
    am_file = fopen(am_file_name, "r");

    /* start of the first pass, each iteration sends one line of code for analysis. */
    while (fgets(line_buffer, MAX_LINE_LEN, am_file)) {
        /*ast = get_pattern(line_buffer);*/
   
        if (ast_array[i].line_type == syntax_error) {
            printf("%s", ast_array[i].syntax_error_msg);
        }else{
            fpass_line(&obj, &IC, &DC, &symbol_table, &ast_array[i]);
        }
        i++;
    }
    add_IC_to_data_sym(&symbol_table,IC);
    /* end of first pass */
     

    /* start second pass */ 
    rewind(am_file);
    IC = BASE_ADDRESS;
    DC = 0;
    i = 0;
    
    while (fgets(line_buffer, MAX_LINE_LEN, am_file)) {
        /*ast = get_pattern(line_buffer);*/
        if (ast_array[i].line_type != syntax_error)
        {
            spass_line(&obj,&entries,&externals, &IC, &DC, &symbol_table, &ast_array[i]);
        }
        i++;
    }
    create_obj_file(&obj, &ob_file, file_name);
    create_ent_file(&entries, &ent_file, file_name);
    create_ext_file(&externals, &ext_file, file_name);
    /*end second pass*/



    free_entries_list(&entries);
    free_externals_list(&externals);
    free_table(&symbol_table);
    free(am_file_name);
    fclose(am_file);
}



int main(int argc, char *argv[]) {
    int i;
    
    for (i = 1; i < argc; i++) {
        compile_file(argv[i]);
    }

    return 0;
}
