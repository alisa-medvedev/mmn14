
#include "lexer.h"

#define DATA_MIN_PARAM -2048
#define DATA_MAX_PARAM 2047
#define INST_PARAM_MIN -512
#define INST_PARAM_MIN 511


/*need to think how to organize all the errors options*/

trie_node instruction_lookup;
trie_node directive_lookup;


/*operand options: I - Integer  L - Label  R - Register*/
static struct instraction_options {
    const char *inst_name;
    int inst_key;
    int inst_name_len;
    const char *source_operand_opt;
    const char *dest_operand_opt;
} instraction_options[16] = {
    {"mov", mov, 3, "ILR", "LR"},
    {"cmp", cmp, 3, "ILR", "ILR"}, 
    {"add", add, 3, "ILR", "LR"},
    {"sub", sub, 3, "ILR", "LR"},
    {"lea", lea, 3, "L", "LR"},

    {"not", not, 3, NULL, "LR"},
    {"clr", clr, 3, NULL, "LR"},
    {"inc", inc, 3, NULL, "LR"},
    {"dec", dec, 3, NULL, "LR"},
    {"jmp", jmp, 3, NULL, "LR"},
    {"bne", bne, 3, NULL, "LR"},
    {"red", red, 3, NULL, "LR"},
    {"prn", prn, 3, NULL, "ILR"},
    {"jsr", jsr, 3, NULL, "LR"},

    {"rts", rts, 3, NULL, NULL},
    {"stop", stop, 4, NULL, NULL}
};


static struct diractive_options {
    const char *diractive_name;
    int dir_key;
    int dir_name_len;
    const char param_opt;
}diractive_options[4] = {
    {"data", dir_data_line, 4, 'I'},
    {"string", dir_string_line, 6, 'S'},
    {"entry", dir_entry_line, 5, 'L'},
    {"extern", dir_extern_line, 6, 'L'}
};

static void init_inst_trie() {
    int i;
    for(i = 0; i < 16; i++) {
        trie_insert(instruction_lookup, instraction_options[i]->inst_name, &instraction_options[i]); 
    }
}

static void init_dir_trie() {
    int i;
    for(i = 0; i < 4; i++) {
        trie_insert(direction_lookup, diractive_options[i]->diractive_name, &diractive_options[i]); 
    }
}


/*Create a utils.c for such func*/
void skip_spaces(char *line) {
    int i = 0;
    while (line[i] == ' ' || line[i] == '\t') {
        i++;
    }
}


enam label_validity_opt{
    non_alpha_first_char,
    contains_non_alpha_numric_char,
    exceeds_possible_length,
    is_valid
};

enam label_validity_opt label_token_check(char *line, char *token, char *end) {
    int char_count = 0;
    if(!isalpha(*line)) {
        return non_alpha_first_char;
    }
    /*label name contains 1 char*/
    if((line+1) == end){
        *token = *(line+1);
        return is_valid;
    };
    while(line != end) {
        if(!isalnum(*line)) {
            return contains_non_alpha_numric_char;
        }
        if(count > LABEL_LEN) {
            return exceeds_possible_length;
        }
        *token = *(line+1);
        char_count++;
        line++;
        token++;
    }
    return is_valid;
}

enam numeric_validity_opt{
    not_a_valid_baginning,
    int_out_of_bounds,
    non_digit_val,
    is_valid
};

static int dirc_parameter_parser(char *line, char *token, char *dirc) {

    if(dirc == diractive_opt_table[0]) {
        while() {
            skip_spaces(line);
            if(*line != '+' && *line != '-' && !isdigit(*line)) {
                return not_a_valid_baginning;
            }
            numeric_token_parser(line, token, DATA_MIN_PARAM, DATA_MAX_PARAM);
            if
        }
        skip_spaces(line);
        if()
        numeric_token_parser(line, token, DATA_MIN_PARAM, DATA_MAX_PARAM);
    }
    if(dirc == diractive_opt_table[1]) {

    }
    if(dirc == diractive_opt_table[2]) {

    }
    if(dirc == diractive_opt_table[3]) {

    }


}


char inst_operand_parser(char *line) {
    if(*line == '@') {
        return 'R';
    }
    else if(isalpha(*line)) {
        return 'L';
    }
    else if(isdigit(*line) || *line == '+' || *line == '-') {
        return 'I';
    }        
    else {
        return NULL;
    }
}

static enam data_dirc_validity_opt get_numeric_token(char *line, char *token, int min, int max) {
    int num_op, sign;
    sign = (*line == '-') ? -1 : 1;
    if(*line == '+' || *line == '-' ) {
        *token = *line;
        line++;
        token++;
    }
    for(num_op = 0; isdigit(*line); line++){
        num_op = 10 * num_op + (*line - '0');
        if((sign == -1 && sign * num_op < min) || (sign == 1 && sign * num_op > max)) {
            return int_out_of_bounds;
        }
        *token = *line;
        token++;
    }
}


struct syntex_tree get_pattern(char *line) {

    struct syntex_tree ast = {0};
    char *dir;
    struct instraction_options *inst = NULL;
    struct diractive_options *dir = NULL;
    char *temp;
    char operand_type;
    char token[MAX_STRING_LEN] = {0};
    enam label_validity_opt label_check;

    init_inst_trie();
    init_dir_trie();
    skip_spaces(line);
    if((temp = strchr(line,':')) != NULL) {/*If we have ':' there has to be a label declaration before*/
        if((label_check = label_token_check(line, token, temp)) == is_valid) {
            strcpy(ast.label, token);
            token[0] = '\0';
        }   
        else{
        if(label_check == non_alpha_first_char) {
            strcpy(ast.syntax_err,"The first characted of a label is not a letter.");
        }
        if(label_check == contains_non_alpha_numric_char) {
            strcpy(ast.syntax_err,"Label contains non alpha numeric characters.");
        }
        if(label_check == exceeds_possible_length) {
            strcpy(ast.syntax_err,"Label excedes the max label length.");
        }
        ast.union_option = ast.syntax_err;
        return ast;
        }
    }
   skip_spaces(line);
   if(ast.label[0] && *line == '\0'){
    strcpy(ast.syntax_err,"Empty line after a label declaration.");
    ast.union_option = ast.syntax_err;
    return ast;
   }
   if (*(line+1) == '.') {
    line++;
    dir = search_trie(directive_lookup, line);
    if(dir == NULL){
        strcpy(ast.syntax_err,"Undefined directory.");
        ast.union_option = ast.syntax_err;
        return ast;
    }
    else{
        skip_directive /*will correct latter*/
        skip_spaces(line);
        if(*line == '\0'){
            strcpy(ast.syntax_err,"Empty line after a directive declaration.");
            ast.union_option = ast.syntax_err;
            return ast;
        }
        dirc_parameter_parser(line, token, dirc)


    }
    ast.union_option = ast.dir_line;
   }
   else{
        inst = search_trie(instruction_lookup, line);
        if(inst == NULL){
            strcpy(ast.syntax_err,"Undefined instraction.");
            ast.union_option = ast.syntax_err;
            return ast;
        }
        line = line + inst->name_len;
        skip_spaces(line);
        if(*line == '\0') {
            if (inst->dest_operand_opt != NULL) {
                strcpy(ast.syntax_err,"Missing operand.");
                ast.union_option = ast.syntax_err;
                return ast;
            }
            ast.union_option = ast.inst_line;
            strcpy(ast.inst_name, inst->inst_name);
            ast.inst_opperands_option = {no_op, no_op};
            return ast;
        }

        if((temp = strchr(line,',')) != NULL ) {
            if(instraction_options.*source_operand_opt == NULL) {
                strcpy(ast.syntax_err,"Too many operators for this instraction.");
                ast.union_option = ast.syntax_err;
                return ast;
            }



        }

   }




}
