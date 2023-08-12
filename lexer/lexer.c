
#include "lexer.h"

#define DATA_MIN_PARAM -2048
#define DATA_MAX_PARAM 2047
#define INST_PARAM_MIN -512
#define INST_PARAM_MIN 511


trie_node instruction_lookup;
trie_node directive_lookup;

/*define a boolean type*/
typedef enam boolean {
    FALSE = 0, TRUE = 1
} boolean;

void skip_spaces(char *line) {
    int i = 0;
    while (line[i] == ' ' || line[i] == '\t') {
        i++;
    }
}

static struct instraction_options {
    const char *inst_name;
    int inst_key;
    int *source_operand_opt;
    int *dest_operand_opt;
} instraction_options[16] = {
    {"mov", mov, {const_op, label_op, register_op}, {label_op, register_op}},
    {"cmp", cmp, {const_op, label_op, register_op}, {const_op, label_op, register_op}}, 
    {"add", add, {const_op, label_op, register_op}, {label_op, register_op}},
    {"sub", sub, {const_op, label_op, register_op}, {label_op, register_op}},
    {"lea", lea, {label_op}, {label_op, register_op}},

    {"not", not, NULL, {label_op, register_op}},
    {"clr", clr, NULL, {label_op, register_op}},
    {"inc", inc, NULL, {label_op, register_op}},
    {"dec", dec, NULL, {label_op, register_op}},
    {"jmp", jmp, NULL, {label_op, register_op}},
    {"bne", bne, NULL, {label_op, register_op}},
    {"red", red, NULL, {label_op, register_op}},
    {"prn", prn, NULL, {label_op, register_op}},
    {"jsr", jsr, NULL, {label_op, register_op}},

    {"rts", rts, NULL, NULL},
    {"stop", stop, NULL, NULL}
};

/*L - label I - array of integers S - string*/
static struct diractive_options {
    const char *diractive_name;
    int dir_key;
    const char dir_param_type;
}diractive_options[4] = {
    {"entry", dir_entry_line, 'L'},
    {"extern", dir_extern_line, 'L'},
    {"data", dir_data_line, 'I'},
    {"string", dir_string_line, 'S'}
};

static boolean label_parser(char *p1, char *p2, char *lbl, struct syntex_tree *ast) {
    int count = 0;
    if(!isalpha(*p1)) {
        strcpy(ast->syntax_error,"The first character of a label is not a letter.");
        return FALSE;
    }
    *lbl = *p1;
    p1++;
    lbl++;
    while(p1 != p2) {
        if(!isalnum(*p1)) {
            strcpy(ast->syntax_error,"Label contains non alpha numeric characters.");
            return FALSE;
        }
        if(count == LABEL_LEN) {
            strcpy(ast->syntax_error,"Label excedes the max label length.");
            return FALSE;
        }
        *lbl = *p1;
        count++;
        p1++;
        lbl++;
    }
    return TRUE;
}

static boolean dir_operand_parser(char *p1, struct syntex_tree *ast, struct directive_options *dir) {
    char *s;
    int *n,i;
    char str[MAX_STRING_LEN] = {0};
    s = str;
    int int_arr[MAX_NUM_ARR_LEN];
    n = int_arr;

    if(dir->dir_param_type == 'L') {
        if(ast->label[0] != '\0') {
            strncpy(ast->label, str, LABEL_LEN); /*label declaration before .entry/.extern have to be ignored*/
        }
        p2 = p1;
        for( ; *p2 != ' ' && *p2 != '\t' && *p2 != '\0'; p2++) {
            ;
        }
        if(label_parser(p1, p2, s, &ast)) {
            strcpy(ast->label, str);
            return TRUE;
        }
        return FALSE;
    }
    else if(dir->dir_param_type == 'I') {
        do {
            if(i == MAX_NUM_ARR_LEN) {
                (ast->syntax_error,"too many operands");
            }
            skip_spaces(p1);
            if(*p1 != '-' && *p1 != '+' && !isdigit(*p1)) {
                (ast->syntax_error,"invalid operand");
                return FALSE;
            }
            if(!numeric_operand_parser(p, &int_arr[i], DATA_MIN_PARAM, DATA_MAX_PARAM)) {
                (ast->syntax_error,"invalid operand");
                return FALSE;
            }
            i++;
            p2 = p1;
            } while(p2 = strchr(p1, ','));
            return TRUE;
    }
    else {
        if(*p1 != '"') {
            strcpy(ast->syntax_error,"invalid start of string directive operand");
            return FALSE;
        }
        p2 = p1 + 1;
        p2 = strchr(p1, '"');
        if(!p2) {
            strcpy(ast->syntax_error,"invalid ending of string directive operand");
            return FALSE;
        }
        for( ; p1 != p2 && s != '\0'; p1++, s++) {
            *s = *p1;
        }
        if(skip_spaces(p1) != '\0') {
            strcpy(ast->syntax_error,"invalid operand");
            return FALSE;
        }
        strcpy(ast->inst_or_dir.dir_line.dir_info, str);
        return TRUE;
    }

}

static boolean inst_line_parser(char *p1, char *p2, struct syntex_tree *ast, struct instraction_options *inst) {
    int int_op;
    int reg_num;
    char label_op[LABEL_LEN]; 

    /*check the case of 0 parameters*/
    if(p1 == '\0') {
        if(inst->source_operand_opt == NULL && inst->dest_operand_opt == NULL) {
            ast->inst_or_dir.inst_line.inst_opperands_option = {no_op, no_op};
            return TRUE;
        }
        strcpy(ast->syntax_error,"missing parameters");
        return FALSE;
    }
    p2 = strchr(line, ',');
    /*if ',' found - there should be at least 2 parameters*/
    if(p2) {
        if(inst->source_operand_opt == NULL) {
            strcpy(ast->syntax_error,"instraction --- recieves only 1 or none parameters");
            return FALSE;
        }
        inst_operand_parser(p1, &int_op, &reg_num, &label_op, ast, inst->source_operand_opt)/*first get source operand*/
    }
    return_inst_param_type(p1, &int_op, &reg_num, &label_op ast, inst->dest_operand_opt)/*get dest operand*/
    return TRUE
}

static boolean inst_operand_parser(char *p1,char *p2, int *int_op, int *reg_num, char *label_op, struct syntex_tree *ast, struct instraction_options **inst) {
    char *label_end;
    if(*p1 == '-' || *p1 == '+' || isdigit(*p1)) {
        if(numeric_operand_parser(p, int_op, INST_PARAM_MIN, INST_PARAM_MAX)) {
            return TRUE;
        }
    }
    else if(*p1 == '@') {
        if(++*p1 != 'r') {
            return -1;
        }
        p1++;
        if(*p1 > 47 && *p1 < 56) {
            *reg_num = *p1 - '0';
            return register_op;
        }
        return -1;
    }
    else if(isalpha(*p1)) {
        return label_op;
    }
    else {
        return -1;
    }
}

/**/
static boolean numeric_operand_parser(char *p, int *num_op, int min, int max) {
    int sign;
    sign = (*p == '-') ? -1 : 1;
    if(!isdigit(*p)) {
        p++;
    }
    for(*num_op = 0; isdigit(*p); p++){
        *num_op = 10 * (*num_op) + (*p - '0');
        if((sign * *num_op < min) || (sign * *num_op > max)) {
            return FALSE;
        }
    }
    return TRUE;
}


struct syntex_tree get_pattern(char *line) {

    char *p1,*p2;
    char dcl_lbl[LABEL_LEN] = {0};
    struct syntex_tree ast = {0};
    struct instraction_options *inst = NULL;
    struct diractive_options *dir = NULL;

    init_inst_trie();
    init_dir_trie();

    skip_spaces(line);
    p2 = strchr(line, ':');
    if(p2) {
        p1 = line;
        if(!label_parser(p1, p2, &dcl_lbl, &ast)) {
            ast.line_type = syntax_error;
            return ast;
        }
        strcpy(ast.label, dcl_lbl);
        line = p2 + 1;
        skip_spaces(line);
        if(*line == '\0') {
            strcpy(ast.syntax_error,"Empty line after a label declaration.");
            ast.line_type = syntax_error;
            return ast;
        }
    }
    if(*line == '.') {
        line++;
        dir = search_trie(directive_lookup, line);
        if(dir == NULL){
            strcpy(ast.syntax_error,"Undefined directory.");
            ast.line_type = syntax_error;
            return ast;
        }
        ast.line_type = dir_line;
        ast.inst_or_dir.dir_line.dir_option = dir->dir_key;
        skip_spaces(line);
        if(!dir_operand_parser(line, &ast, dir)) {
            ast.line_type = syntax_error;
            return ast;
        }
    }
    else{
        inst = search_trie(instruction_lookup, line);
        if(inst == NULL){
            strcpy(ast.syntax_error,"Undefined instraction.");
            ast.line_type = syntax_err;
            return ast;
        }
        ast->inst_or_dir.inst_line.inst_name = inst->inst_key;
        skip_spaces(line);
        p1 = line;
        p2 = p1;
        if(!inst_operand_parser(p1, p2, token, ast, inst)) {
            ast.line_type = syntax_err;
            return ast;
        }
        ast->line_type = inst_line;
        return ast;
    }

}