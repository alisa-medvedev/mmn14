
#include "lexer.h"

trie_node instruction_lookup;
trie_node direction_lookup;


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
}diractive_opt_table[4] = {
    {"data",NULL},
    {"string",NULL},
    {"entry",NULL},
    {"extern",NULL}
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
    while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r') {
        i++;
    }
}


enam label_validity_opt{
    non_alpha_first_char,
    contains_non_alpha_numric_char,
    exceeds_possible_length,
    is_valid
};

enam label_validity_opt label_token_check(char *label) {
    int char_count = 0;
    if(!isalpha(*label)) {
        return non_alpha_first_char;
    }
    if((*label++) == ':'){
        return is_valid;
    };
    while(*label != ':') {
        if(!isalnum(*label)) {
            return contains_non_alpha_numric_char;
        }
        if(count > LABEL_LEN) {
            return exceeds_possible_length;
        }
        char_count++;
        label++;
    }
    return is_valid;
}


struct syntex_tree get_pattern(char *line){

    int token_len;
    struct syntex_tree ast = {0};
    char *stkn;
    char *etkn;
    char token[MAX_STRING_LEN] = {0};
    enam label_validity_opt label_check;

    init_inst_trie()
    init_dir_trie()
    skip_spaces(line);
    stkn = line;
    etkn = strchr(line,':'); /*If we have ':' there has to be a label declaration before*/
    if(etkn && ((label_check =  label_token_check(line)) == is_valid)) {
        token_len = line - stkn;
        memcpy(ast.label, stkn, token_len);
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
   line++;
   skip_spaces(line);
   if(ast.label[0] && *line == '\0'){
    strcpy(ast.syntax_err,"Empty line after a label declaration.");
    ast.union_option = ast.syntax_err;
    return ast;
   }
   if (*(line+1) == '.') {
    line++;
    search_trie(direction_lookup, line);
   }
   else{
    search_trie(instruction_lookup, line);
   }




}

static int numeric_token_parser(char *line, int min, int max) {

}


static int operand_token_parser(char *line) {

}