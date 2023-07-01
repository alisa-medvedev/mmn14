#include "data_structs.h"
#include <stdio.h>
#include <stdlib.h>

#define TRIE_BASE_CHAR ' '

/*trie data structure implementation*/

struct trie_node {
    char current;
    void * end_of_str_ctx;/*end of a string*/
    struct trie_node * next[96];
};

struct trie {
    struct trie_node * next[95];
};

/*returns pointer to a node*/
static struct trie_node *internal_trie_exists(struct trie_node * node_i,const char * string) {
    if(node_i == NULL)
        return NULL;
    if(*string == '\0' && node_i->end_of_str_ctx != NULL){
        return node_i;
    }
    return internal_trie_exists(node_i->next[(*string) - TRIE_BASE_CHAR],string: string +1);
}


Trie trie() {
    return calloc(nmemb: 1,size: sizeof(struct trie));
}

/*checks existance of a word*/
int trie_exists(Trie trie,const char *string) {
    struct trie_node * find_node = internal_trie_exists(node_i:trie->next[(*string) - TRIE_BASE_CHAR],string: string+1);
    return internal_trie_exists(node_i: trie->next[(*string) - TRIE_BASE_CHAR],string: string+1) == NULL ? 0:1; 
}

/*insertion to the trie*/
const char *trie_insert(Trie trie,const char *string,void * end_of_str_ctx) {
    struct trie_node ** iterator = &trie->next[(*string) - TRIE_BASE_CHAR];
    while(*string != '\0') {
        if(*iterator == NULL) {
            (*iterator) = calloc(1,size: sizeof(struct trie_node));
            if(*iterator == NULL)
                return NULL;
        }
        string++;
        iterator = &(*iterator)->next[(*string) - TRIE_BASE_CHAR];
    }
    (*iterator)->end_of_str_ctx = end_of_str_ctx;
    return string;
}

/*delete string - delete later because we don't have to delete strings in our program*/
void trie_delete(Trie trie,const void *string) {

}


/*memory release*/
void trie_destroy(Trie *trie) {

}