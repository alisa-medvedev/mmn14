#include "trie.h"
#include <stdio.h>
#include <stdlib.h>

#define TRIE_BASE_CHAR ' '
#define N 95 /*number of possible chars*/

struct trie_node {
    struct trie_node *children[N];
    void *end_of_str_cnx;
};

/*
struct Trie {
    trie_node *children[N];
};*/
/*trie_node *create_trie() {

}*/

trie_node *make_trienode() {
    /*Memory allocation for a trie_node*/
    int i;
    trie_node *node = NULL;
    node = (trie_node *)malloc(sizeof(trie_node));
    if(node != NULL) {
        for (i = 0; i < N; i++) {
            node->children[i] = NULL;
        }
        node->end_of_str_cnx = NULL;
    }
    return node;
}

/*Insertion of a string onto the trie*/
const char *trie_insert(trie_node *root,const char *string,void *end_of_str_cnx) {
    trie_node *temp = root;
    while(*string != '\0') {
        if(temp->children[*string - TRIE_BASE_CHAR] == NULL) {
            temp->children[*string - TRIE_BASE_CHAR] = make_trienode();
        }
        temp = temp->children[*string - TRIE_BASE_CHAR];
            if(temp == NULL) {
                return NULL;
            }
        string++;
    }
    temp->end_of_str_cnx = end_of_str_cnx;
    return string;
}

/*
    trie_node **temp = &root->children[(*string) - TRIE_BASE_CHAR];
    while(1) {
        if(*temp == NULL) {
            *temp = make_trienode();
            if(*temp == NULL)
                return NULL;
        }
        string++;
        if(*string != '\0')
            temp = &(*temp)->children[(*string) - TRIE_BASE_CHAR];
        else
            break;
    }*/
    /*At the end of the string */
  /*  (*temp)->end_of_str = end_of_str;
    return string;
}
*/

/*Checks the existance of a string*/
void *search_trie(trie_node *root,const char *string) {
    trie_node* temp = root;
    while(*string != '\0') {
        if (temp->children[(*string) - TRIE_BASE_CHAR] == NULL) {
            return NULL;
        }
        temp = temp->children[(*string) - TRIE_BASE_CHAR];
        string++;
    }
    if (temp != NULL && temp->end_of_str_cnx != NULL) {
        return temp->end_of_str_cnx;
    }
    return NULL;
}

/*memory release*/
void trie_destroy(trie_node *root) {
    int i;
    trie_node *node = root;
    for(i = 0; i < N; i++) {
        if((node->children[i]) != NULL) {
            trie_destroy(node->children[i]);
            node->children[i] = NULL;
        }
    }
    free(node);
}

