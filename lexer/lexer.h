#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#define ERROR_MSG_LEN 150
#define LABEL_LEN 31
#define MAX_STRING_LEN 200
#define MAX_NUM_ARR_LEN 60
#define ERROR_LEN 150


struct syntex_tree
{
    char syntax_error_msg[ERROR_LEN];
    char label[LABEL_LEN];

    enum{
        syntax_error,
        dir_line,
        inst_line
    } line_type;
    
    union{
        struct{
            enum{
                dir_entry_line,
                dir_extern_line,
                dir_data_line,
                dir_string_line
            } dir_option;
            union{
                struct
                {
                    int num_arr[MAX_NUM_ARR_LEN];
                    int num_arr_len;
                } num_array;
                char str[MAX_STRING_LEN];
            } dir_info;
            
        } dir_line;

        struct{
            enum{
                /*instructions with source and destenetion opperands*/
                mov, cmp, add, sub, lea,
                /*instructions with only destenetion opperand*/
                not, clr, inc, dec, jmp, bne, red, prn, jsr,
                /*instructions with no opperands*/
                rts, stop
            } inst_name;
            enum{
                const_op = 1,
                register_op = 5,
                label_op = 3,
                no_op = 0
            } inst_opperands_option[2];
            union
            {
                int const_num;
                int reg_num;
                char label[LABEL_LEN];
            } inst_opperands[2];
        } inst_line;
    } inst_or_dir;
};

struct syntex_tree get_pattern(char *line);

#endif /* SYNTAX_TREE_H */
