#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structures/object_file/object_file.h"

#define BASE_ADDR 100

void add_to_code_img(struct object_file *obj, int machine_word, int IC) {
    obj->code_img[IC - BASE_ADDR] = machine_word;
    obj->code_len++;
}

void add_to_data_img(struct object_file *obj, int machine_word, int DC) {
    obj->data_img[DC] = machine_word;
    obj->data_len++;
}

void add_entry(struct entrie** entries, const char* label, int address) {
    struct entrie* new_entry = (struct entrie*)malloc(sizeof(struct entrie));
    strncpy(new_entry->label, label, LABEL_LEN);
    new_entry->label[LABEL_LEN - 1] = '\0'; 
    new_entry->address = address;
    new_entry->next = NULL;

    if (*entries == NULL) {
        *entries = new_entry;
    } else {
        struct entrie* current = *entries;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_entry;
    }
}

void add_external(struct external** externals, const char* label, int address) {
    struct external* new_external = (struct external*)malloc(sizeof(struct external));
    strncpy(new_external->label, label, LABEL_LEN);
    new_external->label[LABEL_LEN - 1] = '\0'; 
    new_external->address = address;
    new_external->next = NULL;

    if (*externals == NULL) {
        *externals = new_external;
    } else {
        struct external* current = *externals;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_external;
    }
}
void free_entries_list(struct entrie** entries) {
    while (*entries != NULL) {
        struct entrie* current = *entries;
        *entries = (*entries)->next;
        free(current);
    }
}

void free_externals_list(struct external** externals) {
    while (*externals != NULL) {
        struct external* current = *externals;
        *externals = (*externals)->next;
        free(current);
    }
}