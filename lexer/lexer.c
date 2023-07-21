
#include "lexer.h"

/*need to think how to organize all the errors options*/

trie_node instruction_lookup;
trie_node directive_lookup;


/*operand options: I - Integer  L - Label  R - Register*/
static struct instraction_options {
    const char *inst_name;
    int key;
    const char *source_operand_opt;
    const char *dest_operand_opt;
} instraction_opt_tab[16] = {
    {"mov", mov, "ILR", "LR"},
    {"cmp", cmp, "ILR", "ILR"}, 
    {"add", add, "ILR", "LR"},
    {"sub", sub, "ILR", "LR"},
    {"lea", lea, "L", "LR"},

    {"not", not, NULL, "LR"},
    {"clr", clr, NULL, "LR"},
    {"inc", inc, NULL, "LR"},
    {"dec", dec, NULL, "LR"},
    {"jmp", jmp, NULL, "LR"},
    {"bne", bne, NULL, "LR"},
    {"red", red, NULL, "LR"},
    {"prn", prn, NULL, "ILR"},
    {"jsr", jsr, NULL, "LR"},

    {"rts", rts, NULL, NULL},
    {"stop", stop, NULL, NULL}
};


static struct diractive_options {
    const char *diractive_name;
    int key;
    const char param_opt;
}diractive_opt_table[4] = {
    {"data",NULL,'I'},
    {"string",NULL,'S'},
    {"entry",NULL,'L'},
    {"extern",NULL,'L'}
};

static void init_inst_trie() {
    int i;
    for(i = 0; i < 16; i++) {
        trie_insert(instruction_lookup, instraction_opt_tab[i]->inst_name, &instraction_opt_tab[i]); 
    }
}

static void init_dir_trie() {
    int i;
    for(i = 0; i < 4; i++) {
        trie_insert(direction_lookup, diractive_opt_table[i]->diractive_name, &diractive_opt_table[i]); 
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

enam label_validity_opt label_token_check(char *line, char *token) {
    int char_count = 0;
    if(!isalpha(*line)) {
        return non_alpha_first_char;
    }
    /*label name contains 1 char*/
    if((*(line+1)) == ':'){
        *token = *(line+1);
        return is_valid;
    };
    while(*line != ':') {
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
            numeric_token_parser(line, token, -2048, 2047);
        }
        skip_spaces(line);
        if(   )
        numeric_token_parser(line, token, -2048, 2047);
    }
    if(dirc == diractive_opt_table[1]) {

    }
    if(dirc == diractive_opt_table[2]) {

    }
    if(dirc == diractive_opt_table[3]) {

    }


}

static enam data_dirc_validity_opt numeric_token_parser(char *line, char *token, int min, int max) {
    int char_count = 0;
    if(*line == '+' || *line == '-' ) {
        *token = *line;
        line++;
        token++;
    }
    while(isdigit(*line)) {
        char_count++;
        if(char_count > 4) {
            return int_out_of_bounds;
        }
        *token = *line;
        line++;
        token++;
    }
}


struct syntex_tree get_pattern(char *line){

    struct syntex_tree ast = {0};
    char *etkn;
    char *dir;
    char *inst;
    char token[MAX_STRING_LEN] = {0};
    enam label_validity_opt label_check;

    init_inst_trie()
    init_dir_trie()
    skip_spaces(line);
    etkn = strchr(line,':'); /*If we have ':' there has to be a label declaration before*/
    if(etkn && ((label_check =  label_token_check(line, token)) == is_valid)) {
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
   }




}



static int operand_token_parser(char *line) {

}