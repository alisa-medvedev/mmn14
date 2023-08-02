
#include "lexer.h"
#include "base.h"

#define DATA_MIN_PARAM -2048
#define DATA_MAX_PARAM 2047
#define INST_PARAM_MIN -512
#define INST_PARAM_MIN 511


trie_node instruction_lookup;
trie_node directive_lookup;

/*define a boolean type*/
typedef enam booleans {
    FALSE = 0, TRUE = 1
} boolean;

void skip_spaces(char *line) {
    int i = 0;
    while (line[i] == ' ' || line[i] == '\t') {
        i++;
    }
}

/*operand options: I - Integer  L - Label  R - Register*/
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


static struct diractive_options {
    const char *diractive_name;
    int dir_key;
    int param_opt;
}diractive_options[4] = {
    {"data", dir_data_line, 'I'},
    {"string", dir_string_line, 'S'},
    {"entry", dir_entry_line, 'L'},
    {"extern", dir_extern_line, 'L'}
};

static boolean label_parser(char *p1, char *p2, char *token, struct syntex_tree *ast) {
    int count = 0;
    if(!isalpha(*p1)) {
        strcpy(ast->syntax_error,"The first character of a label is not a letter.");
        return FALSE;
    }
    *token = *p1;
    p1++;
    token++;
    while(p1 != p2) {
        if(!isalnum(*p1)) {
            strcpy(ast->syntax_error,"Label contains non alpha numeric characters.");
            return FALSE;
        }
        if(count == LABEL_LEN) {
            strcpy(ast->syntax_error,"Label excedes the max label length.");
            return FALSE;
        }
        *token = *p1;
        count++;
        p1++;
        token++;
    }
    return TRUE;
}

static boolean inst_operand_parser(char *p1, char *p2, char *token, struct syntex_tree *ast, struct instraction_options *inst) {
    char operand_type;
    int operand_num;
    int int_operand;
    /*check the case of 0 parameters*/
    if(p1 == '\0') {
        if(inst->source_operand_opt == NULL && inst->onst char dest_operand_opt) {
            return TRUE;
        }
        strcpy(ast->syntax_error,"missing parameters");
        return FALSE;
    }
    p2 = strchr(line, ',');
    /*if ',' found - there should be at least 2 parameters*/
    if(p2) {
        if(inst->source_operand_opt != NULL) {
            strcpy(ast->syntax_error,"instraction --- recieves only 1 or none parameters");
            return FALSE;
        }
        operand_num = 2;
    }
    else {
        operand_num = 1;
    }
    while(operand_num < 3) {
        operand_type = return_param_type(p1, )
        switch(*curr) {
            case 'I':
                const_op = numeric_operand_parser(p1, INST_PARAM_MIN, INST_PARAM_MAX);
            case 'L':
            case 'R':
                if(*++p1 != 'r') {
                    strcpy(ast->syntax_error,"Unrecognized parameter");
                    ast.union_option = ast.syntax_error;
                    return FALSE;
                }
                p1++;
                if(*p1 > 47 && *p1 < 56)
            default:
                return FALSE;
        }
    }

}

/**/
static int numeric_operand_parser(char *p, int min, int max) {
    int num_op, sign;
    sign = (*p == '-') ? -1 : 1;
    if(*p == '+' || *p == '-' ) {
        *token = *p;
        p++;
        token++;
    }
    for(num_op = 0; isdigit(*p); p++){
        num_op = 10 * num_op + (*p - '0');
        if((sign == -1 && sign * num_op < min) || (sign == 1 && sign * num_op > max)) {
            return FALSE;
        }
    }
}

char return_param_type(char *p) {
    if(*p == @) {
        return 'R';
    }
    else if(*p == '-' || *p == '+' || isdigit(*p)) {
        return 'I';
    }
    else if(isalpha)
}


struct syntex_tree get_pattern(char *line) {

    char *p1,*p2;
    char token[MAX_STRING_LEN] = {0};
    struct syntex_tree ast = {0};
    struct instraction_options *inst = NULL;
    struct diractive_options *dir = NULL;

    init_inst_trie();
    init_dir_trie();

    skip_spaces(line);
    p2 = strchr(line, ':');
    if(p2) {
        p1 = line;
        if(!label_parser(p1, p2, token, ast)) {
            ast.union_option = ast.syntax_err;
            return ast;
        }
        strcpy(ast.label, token);
        token[0] = '\0';
        line = p2 + 1;
        skip_spaces(line);
        if(*line == '\0') {
            strcpy(ast->syntax_error,"Empty line after a label declaration.");
            ast.line_type = ast.syntax_error;
            return ast;
        }
    }
    if(*line == '.') {
        line++;
        dir = search_trie(directive_lookup, line);
        if(dir == NULL){
            strcpy(ast.syntax_error,"Undefined directory.");
            ast.union_option = ast.syntax_error;
            return ast;
        }
        ast.inst_or_dir.dir_line.dir_option = dir->dir_key;

        /*if we will not have syntax errors*/
        ast.union_option = ast.dir_line;
    }
    else{
        inst = search_trie(instruction_lookup, line);
        if(inst == NULL){
            strcpy(ast.syntax_error,"Undefined instraction.");
            ast.union_option = ast.syntax_error;
            return ast;
        }
        ast.inst_or_dir.inst_line.inst_name = inst->inst_key;
        line = line + (strlen(inst->inst_name[0]) - 1);
        skip_spaces(line);
        p1 = line;
        p2 = p1;
        inst_operand_parser(p1, p2, token, ast, inst);


        /*if we will not have syntax errors*/
        ast.union_option = ast.inst_line;
    }

}