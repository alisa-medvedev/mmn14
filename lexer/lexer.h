

#define ERROR_MSG_LEN 150
#define LABEL_LEN 31
#define MAX_STRING_LEN 200
#define MAX_NUM_ARR_LEN 60
#define ERROR_LEN 150

struct syntex_tree
{
    char syntax_error[ERROR_LEN];
    char label[LABEL_LEN];

    enum {
        inst_line,
        dir_line,
        syntax_err
    }union_option;
    
    union{

        struct{
            enum{
                dir_data_line,
                dir_string_line,
                dir_entry_line,
                dir_extern_line
            }dir_option;
            union{
                struct
                {
                    int num_array[MAX_NUM_ARR_LEN];
                    int num_arr_len;
                }num_array;
                char str[MAX_STRING_LEN];
                char label[LABEL_LEN];
            }dir_info;
            
        }dir_line;

        struct{
            enum{
                /*instructions with source and destenetion opperands*/
                mov, cmp, add, sub, lea,
                /*instructions with only destenetion opperand*/
                not, clr, inc, dec, jmp, bne, red, prn, jsr,
                /*instruction with no opperands*/
                rts, stop
            }inst_name;
            enum{
                const_op,
                register_op,
                label_op,
                no_op
            }inst_opperands_option[2];
            union
            {
                int const_num;
                int reg_num;
                char label[LABEL_LEN];
            }inst_opperands[2];
        }isnt_line;
          

    }inst_or_dir;
    
};


struct syntex_tree get_pattern(char *line);