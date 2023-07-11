
#include "lexer.h"

trie_node instruction_lookup = NULL;
trie_node direction_lookup = NULL;

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
}

/*Create a utils.c for such func*/
void skip_spaces(char *line) {
    int i = 0;
    while (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r') {
        i++;
    }
}



struct syntex_tree get_pattern(char *line){
   struct syntex_tree ast = {0};
   char token[MAX_STRING_LEN] = {0};


   skip_spaces(line);



}

static int numeric_token_parser(char *line, int min, int max) {

}

static int label_token_parser(char *line) {

}

static int operand_token_parser(char *line) {

}