#ifndef OBJECT_FILE_H
#define OBJECT_FILE_H

#define MAX_CODE_LEN 1000
#define LABEL_LEN 31

#include <stdio.h>

struct object_file {
    int code_img[MAX_CODE_LEN];
    int data_img[MAX_CODE_LEN];
    int code_len;
    int data_len;
};

struct entrie {
    char label[LABEL_LEN];
    int address;
    struct entrie* next;
};

struct external {
    char label[LABEL_LEN];
    int address;
    struct external* next;
};


void add_to_code_img(struct object_file *obj, int machine_word, int IC);
void add_to_data_img(struct object_file *obj, int machine_word, int DC);
void add_entry(struct entrie** entries, const char* label, int address);
void add_external(struct external** externals, const char* label, int address);
void free_entries_list(struct entrie** entries);
void free_externals_list(struct external** externals);

#endif