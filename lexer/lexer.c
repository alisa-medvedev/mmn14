#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

#define IS_CONST_OP_START(S) ((S == '+') || (S == '-') || (isdigit(S))) ? 1:0
#define SKIP_SPACES(S)()

#define DATA_MIN_PARAM -2048
#define DATA_MAX_PARAM 2047
#define INST_PARAM_MIN -512
#define INST_PARAM_MAX 511


/*define a boolean type*/
typedef enum boolean {
    FALSE = 0, TRUE = 1
} boolean;

static char *skip_spaces(char *line) {
    char *s = NULL;
    s = line;
    while (*s == ' ' || *s == '\t') {
        s++;
    }
    return s;
}

static int custom_strcmp(char *p1,const char *p2) {
	for( ; *p1 == *p2 && *p1 != '\n'; p1++, p2++) {
        ;
	}
	if(*p2 == '\0') {
		return 0;
    }
	return *p1-*p2;
}


static struct instraction_options {
    const char *instruction_name;
    int inst_key;
    const char *source_operand_opt;
    const char *dest_operand_opt;
} instraction_options[16] = {
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


static struct directive_options {
    const char *directive_name;
    int dir_key;
} directive_options[4] = {
    {"entry", dir_entry_line},
    {"extern", dir_extern_line},
    {"data", dir_data_line},
    {"string", dir_string_line}
};


static struct instraction_options *find_instruction(char *line) {
    int i;
	char *temp = NULL;

    for(i = 0; i < 16; i++) {
		temp = line;
        if(custom_strcmp(temp, instraction_options[i].instruction_name) == 0) {
            return &instraction_options[i];
        }
    }
    return NULL; 
}

static struct directive_options *find_directive(char *line) {
    int i;
	char *temp = NULL;

    for(i = 0; i < 4; i++) {
		temp = line;
         if(custom_strcmp(temp, directive_options[i].directive_name) == 0) {
            return &directive_options[i];
        }
    }
    return NULL; 
}

static boolean numeric_operand_parser(char *p, int *num_op, int min, int max) {
    int sign;
    sign = (*p == '-') ? -1 : 1;
    if(!isdigit(*p)) {
        p++;
    }
    for(*num_op = 0; isdigit(*p); p++){
        *num_op = 10 * (*num_op) + (*p - '0');
        if((sign * (*num_op) < min) || (sign * (*num_op) > max)) {
            return FALSE;
        }
    }
    *num_op = sign * (*num_op);
    return TRUE;
}


static int label_parser(char *p, char *lbl, char *error) {
    int count = 0;
    if(!isalpha(*p)) {
        strcpy(error,"The first character of a label is not a letter.");
        return FALSE;
    }
    *lbl = *p;
    p++;
    lbl++;
    while(isalnum(*p)) {
        if(count == LABEL_LEN) {
            strcpy(error,"Label excedes the max label length.");
            return 0;
        }
        *lbl = *p;
        count++;
        p++;
        lbl++;
    }
    return count + 1;
}

static boolean dir_operand_parser(char *line, struct syntex_tree *ast, struct directive_options *dir) {
    char *p = NULL;
    int i, label_len;
    int curr_num;
    char str[MAX_STRING_LEN] = {0};
    char error[ERROR_MSG_LEN] = {0};

    if(dir->dir_key == dir_entry_line || dir->dir_key == dir_extern_line) {
        if(ast->label[0] != '\0') {
            strncpy(ast->label, str, LABEL_LEN); /*label declaration before .entry/.extern have to be ignored*/
        }
        if((label_len = label_parser(line, str, error)) > 0) {
            line = line + label_len;
            if(*line != ' ' && *line != '\t' && *line != '\n') {
                strcpy(ast->syntax_error,"Error! label has non alpha-numeric chars");
                return FALSE;
            }
            if(*(line = skip_spaces(line)) != '\n') {
                strcpy(ast->syntax_error,"Error! the line is not empty after label operand");
                return FALSE;
            }
            strcpy(ast->label, str);
            return TRUE;
            }
        strcpy(ast->syntax_error, error);
        return FALSE;
    }
    else if(dir->dir_key == dir_data_line) {
        i = 0;
		printf("line %c\n",*line);
        while(*line != '\n') {
			curr_num = 0;
			printf("line %c\n",*line);
            if(i == MAX_NUM_ARR_LEN) {
                strcpy(ast->syntax_error,"too many operands");
                return FALSE;
            }
            if(!IS_CONST_OP_START(*line)) {
                strcpy(ast->syntax_error,"invalid operand");
                return FALSE;
            }
            if(!numeric_operand_parser(line, &curr_num, DATA_MIN_PARAM, DATA_MAX_PARAM)) {
                strcpy(ast->syntax_error,"invalid operand");
                return FALSE;
            }
            line++;
            while(isdigit(*line)){
				line++;
			}
			line = skip_spaces(line);
			if(*line == ',') {
				line++;
				line = skip_spaces(line);
				if(!IS_CONST_OP_START(*line)){
					strcpy(ast->syntax_error,"invalid operand");
					return FALSE;
				}
			}
			ast->inst_or_dir.dir_line.dir_info.num_array.num_arr[i] = curr_num;
            ast->inst_or_dir.dir_line.dir_info.num_array.num_arr_len = i + 1;			
            i++;
        }
		return TRUE;
    }
    else {
        if(*line != '"') {
            strcpy(ast->syntax_error,"Error! invalid start of string directive operand");
            return FALSE;
        }
        line++;
        p = strchr(line, '"');
        if(!p) {
            strcpy(ast->syntax_error,"Error! invalid ending of string directive operand");
            return FALSE;
        }
        for(i = 0 ; line != p; line++, i++) {
            if(i == MAX_STRING_LEN) {
                strcpy(ast->syntax_error,"Error! too long string");
                return FALSE;
            }
            str[i] = *line;
        }
        line++;
        line = skip_spaces(line);
        if(*line != '\n') {
            strcpy(ast->syntax_error,"invalid operand");
            return FALSE;
        }
        strcpy(ast->inst_or_dir.dir_line.dir_info.str, str);
        return TRUE;
    }

}

static char inst_operand_type(char *f_char) {
    if(IS_CONST_OP_START(*f_char)) {
        return 'I';
    }
    else if(*f_char == '@') {
        return 'R';
    }
    else if(isalpha(*f_char)) {
        return 'L';
    }
    else {
        return 'N';
    }
}

static boolean inst_line_parser(char *line, struct syntex_tree *ast, struct instraction_options *inst) {
    char *temp = NULL;
    int int_op;
    int register_num;
    char label_operand[LABEL_LEN] = {0}; 
    char source_operand_type;
    char dest_operand_type;
    char error[ERROR_MSG_LEN] = {0};
    int label_len;

    /*check the case of 0 parameters*/
    if(*line == '\n') {
        if(inst->source_operand_opt == NULL && inst->dest_operand_opt == NULL) {
            ast->inst_or_dir.inst_line.inst_opperands_option[0] = no_op;
            ast->inst_or_dir.inst_line.inst_opperands_option[1] = no_op;
            return TRUE;
        }
        strcpy(ast->syntax_error,"Error! missing parameters");
        return FALSE;
    }
    temp = strchr(line, ',');
        if(temp) {
        if(inst->source_operand_opt == NULL) {
            strcpy(ast->syntax_error,"instraction --- recieves only 1 or none parameters");
            return FALSE;
        }
        source_operand_type = inst_operand_type(line);
        if(strchr(inst->source_operand_opt, source_operand_type) == NULL) {
            strcpy(ast->syntax_error,"operand is not supported");
            return FALSE;
        }
        if(source_operand_type == 'I') {
            if(numeric_operand_parser( line, &int_op, INST_PARAM_MIN, INST_PARAM_MAX)) {
                ast->inst_or_dir.inst_line.inst_opperands_option[0] = const_op;
                ast->inst_or_dir.inst_line.inst_opperands[0].const_num = int_op;
                line++;
                for( ;isdigit(*line) ;line++) {
                    ;
                }
            }
            else {
                strcpy(ast->syntax_error,"Error! parameter is not correct");
                return FALSE;
            }
        }
        else if(source_operand_type == 'R') {
            if(*++line == 'r') {
                line++;
                if(*line > 47 && *line < 56) {
                    register_num = *line - '0';
                    ast->inst_or_dir.inst_line.inst_opperands_option[0] = register_op;
                    ast->inst_or_dir.inst_line.inst_opperands[0].reg_num = register_num;
                    line++;
                }
            }
            else {
                strcpy(ast->syntax_error,"Error! parameter is not correct");
                return FALSE;
            }
        }
        else if(source_operand_type == 'L') {
            if((label_len = label_parser(line, label_operand, error)) > 0) {
                ast->inst_or_dir.inst_line.inst_opperands_option[0] = label_op;
                strcpy(ast->inst_or_dir.inst_line.inst_opperands[0].label, label_operand);
                line = line + label_len;
                label_operand[0] = '\0';
            }
            else {
                strcpy(ast->syntax_error,"Error! parameter is not correct");
                return FALSE;
            }
        }

        else {
                strcpy(ast->syntax_error,"Error! parameter is not correct");
                return FALSE;
        }
        /*
        switch(source_operand_type)
        {
            case 'I':
                if(numeric_operand_parser( line, &int_op, INST_PARAM_MIN, INST_PARAM_MAX)) {
                    ast->inst_or_dir.inst_line.inst_opperands_option[0] = const_op;
                    ast->inst_or_dir.inst_line.inst_opperands[0].const_num = int_op;
                    line++;
                    for( ;isdigit(*line) ;line++) {
                        ;
                    }
                }
                else {
                    strcpy(ast->syntax_error,"Error! parameter is not correct");
                    return FALSE;
                }
                break;
            case 'R':
                if(*++line == 'R') {
                    line++;
                    if(*line > 47 && *line < 56) {
                        register_num = *line - '0';
                        ast->inst_or_dir.inst_line.inst_opperands_option[0] = register_op;
                        ast->inst_or_dir.inst_line.inst_opperands[0].reg_num = register_num;
                        line++;
                    }
                }
                else {
                    strcpy(ast->syntax_error,"Error! parameter is not correct");
                    return FALSE;
                }
                break;
            case 'L':
                if((label_len = label_parser(line, label_operand, error)) > 0) {
                    ast->inst_or_dir.inst_line.inst_opperands_option[0] = label_op;
                    strcpy(ast->inst_or_dir.inst_line.inst_opperands[0].label, label_operand);
                    line = line + label_len;
                    label_operand[0] = '\0';
                }
                else {
                    strcpy(ast->syntax_error,"Error! parameter is not correct");
                    return FALSE;
                }
                break;
            default:
                strcpy(ast->syntax_error,"Error! parameter is not correct");
                return FALSE;
        }*/
        if((line = skip_spaces(line)) != temp) {
            strcpy(ast->syntax_error,"Error! missing a comma between 2 parameters");
            return FALSE;
        }
        line++;
        line = skip_spaces(line);
        }
        dest_operand_type = inst_operand_type(line);
        if(strchr(inst->dest_operand_opt, dest_operand_type) == NULL) {
            strcpy(ast->syntax_error,"Error! operand is not supported");
            return FALSE;
        }
        if(dest_operand_type == 'I') {
            if(numeric_operand_parser( line, &int_op, INST_PARAM_MIN, INST_PARAM_MAX)) {
                ast->inst_or_dir.inst_line.inst_opperands_option[1] = const_op;
                ast->inst_or_dir.inst_line.inst_opperands[1].const_num = int_op;
                line++;
                for( ;isdigit(*line) ;line++) {
                    ;
                }
            }
            else {
                strcpy(ast->syntax_error,"Error! parameter is not correct");
                return FALSE;
            }
        }
        else if(dest_operand_type == 'R') {
            if(*++line == 'R') {
                line++;
                if(*line > 47 && *line < 56) {
                    register_num = *line - '0';
                    ast->inst_or_dir.inst_line.inst_opperands_option[1] = register_op;
                    ast->inst_or_dir.inst_line.inst_opperands[1].reg_num = register_num;
                    line++;
                }
            }
            else {
                strcpy(ast->syntax_error,"Error! parameter is not correct");
                return FALSE;
            }
        }
        else if(dest_operand_type == 'L') {
            if((label_len = label_parser(line, label_operand, error)) > 0) {
                ast->inst_or_dir.inst_line.inst_opperands_option[1] = label_op;
                strcpy(ast->inst_or_dir.inst_line.inst_opperands[1].label, label_operand);
                line = line + label_len;
            }
            else {
                strcpy(ast->syntax_error,"Error! parameter is not correct");
                return FALSE;
            }
        }
        else {
                strcpy(ast->syntax_error,"Error! parameter is not correct");
                return FALSE;
        }
       /* switch(dest_operand_type)
        {
            case 'I':
                if(numeric_operand_parser( line, &int_op, INST_PARAM_MIN, INST_PARAM_MAX)) {
                    ast->inst_or_dir.inst_line.inst_opperands_option[1] = const_op;
                    ast->inst_or_dir.inst_line.inst_opperands[1].const_num = int_op;
                    line++;
                    for( ;isdigit(*line) ;line++) {
                        ;
                    }
                }
                else {
                    strcpy(ast->syntax_error,"Error! parameter is not correct");
                    return FALSE;
                }
                break;
            case 'R':
                if(*++line == 'R') {
                    line++;
                    if(*line > 47 && *line < 56) {
                        register_num = *line - '0';
                        ast->inst_or_dir.inst_line.inst_opperands_option[1] = register_op;
                        ast->inst_or_dir.inst_line.inst_opperands[1].reg_num = register_num;
                        line++;
                    }
                }
                else {
                    strcpy(ast->syntax_error,"Error! parameter is not correct");
                    return FALSE;
                }
                break;
            case 'L':
                if((label_len = label_parser(line, label_operand, error)) > 0) {
                    ast->inst_or_dir.inst_line.inst_opperands_option[1] = label_op;
                    strcpy(ast->inst_or_dir.inst_line.inst_opperands[1].label, label_operand);
                    line = line + label_len;
                }
                else {
                    strcpy(ast->syntax_error,"Error! parameter is not correct");
                    return FALSE;
                }
                break;
            default:
                strcpy(ast->syntax_error,"Error! parameter is not correct");
                return FALSE;
        }*/
        if(*(line = skip_spaces(line)) != '\n') {
            strcpy(ast->syntax_error,"Error! parameter is not correct");
            return FALSE;
        }
        return TRUE;
        
}


struct syntex_tree get_pattern(char *line) {

    char *p1 = NULL;
    char *p2 = NULL;
    char dcl_lbl[LABEL_LEN] = {0};
    char error[ERROR_MSG_LEN] = {0};
    struct syntex_tree ast = {0};
    struct instraction_options *inst = NULL;
    struct directive_options *dir = NULL;

    line = skip_spaces(line);
    p2 = strchr(line, ':');
    if(p2) {
        p1 = line;
        if((line += label_parser(p1, dcl_lbl, error)) != p2) {
            strcpy(ast.syntax_error, error);
            ast.line_type = syntax_error;
            return ast;
        }
        strcpy(ast.label, dcl_lbl);
        line++;
        line = skip_spaces(line);
        if(*line == '\n') {
            strcpy(ast.syntax_error,"Empty line after a label declaration.");
            ast.line_type = syntax_error;
            return ast;
        }
    }
    if(*line == '.') {
        line++;
        dir = find_directive(line);
        if(!dir){
            strcpy(ast.syntax_error,"Undefined directive.");
            ast.line_type = syntax_error;
            return ast;
        }
        line = line + strlen(dir->directive_name);
        line = skip_spaces(line);
        if(!dir_operand_parser(line, &ast, dir)) {
            ast.line_type = syntax_error;
            return ast;
        }
        ast.line_type = dir_line;
        ast.inst_or_dir.dir_line.dir_option = dir->dir_key;
    }
    else{
        inst = find_instruction(line);
        if(!inst){
            strcpy(ast.syntax_error,"Undefined instruction.");
            ast.line_type = syntax_error;
            return ast;
        }
        line = line + strlen(inst->instruction_name);
        ast.inst_or_dir.inst_line.inst_name = inst->inst_key;
        line = skip_spaces(line);
        if(!inst_line_parser(line, &ast, inst)) {
            ast.line_type = syntax_error;
            return ast;
        }
        ast.line_type = inst_line;
    }
    return ast;
}
