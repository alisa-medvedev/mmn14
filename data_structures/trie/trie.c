#include "trie.h"
#include <stdio.h>
#include <stdlib.h>

#define TRIE_BASE_CHAR '0'
#define N 75 /*number of chars from '0' to 'z' in ASCII Table*/

struct trie_node {
    char current;
    void * end_of_str_ctx;/*end of a string context*/
    struct trie_node * next[N];
    int end_of_str;
};

/*returns pointer to a node*/
static trie_node *internal_trie_exists(trie_node * node_i,const char * string) {
    if(node_i == NULL)
        return NULL;
    if(*string == '\0' && node_i->end_of_str_ctx != NULL){
        return node_i;
    }
    return internal_trie_exists(node_i: node_i->next[(*string) - TRIE_BASE_CHAR],string++);
}

trie_node* make_trie(void) {
    return (trie_node*) calloc(1, sizeof(trie_node));
} 

/*insertion to the trie*/
const char *trie_insert(trie_node *root,const char *string,void *end_of_str_ctx) {
    trie_node ** iterator = &root->next[(*string) - TRIE_BASE_CHAR];
    while(*string != '\0') {
        if(*iterator == NULL) {
            (*iterator) = calloc(1, sizeof(trie_node));
            if(*iterator == NULL)
                return NULL;
        }
        string++;
        iterator = &(*iterator)->next[(*string) - TRIE_BASE_CHAR];
    }
    (*iterator)->end_of_str_ctx = end_of_str_ctx;
    return string;
}

/*checks existance of a word*/
void * trie_exists(trie_node *root,const char *string) {
    trie_node * find_node = internal_trie_exists(node_i:root->next[(*string) - TRIE_BASE_CHAR],string: string+1);
    return find_node == NULL ? NULL : find_node->end_of_str_ctx;
}


/*memory release*/
void trie_destroy(trie_node *root) {

}