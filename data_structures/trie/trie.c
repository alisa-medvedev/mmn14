#include "trie.h"
#include <stdio.h>
#include <stdlib.h>

#define TRIE_BASE_CHAR ' '
#define N 95 /*number of possible chars*/

struct trie_node {
    struct trie_node *children[N];
    void *end_of_str;
};

trie_node *make_trienode() {
    /*Memory allocation for a trie_node*/
    return (trie_node *)calloc(1, sizeof(trie_node));
} 

/*Insertion of a string onto the trie*/
const char *trie_insert(trie_node *root,const char *string,void *end_of_str) {
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
    }
    /*At the end of the string */
    (*temp)->end_of_str = end_of_str;
    return string;
}


/*Checks the existance of a string*/
void *search_trie(trie_node *root,const char *string) {
    trie_node* temp = root;
    for(; *string !='\0'; string++)
    {
        if (temp->children[(*string) - TRIE_BASE_CHAR] == NULL)
            return NULL;
        temp = temp->children[(*string) - TRIE_BASE_CHAR];
    }
    if (temp != NULL && temp->end_of_str != NULL)
        return temp = temp->end_of_str;
    return NULL;
}

/*memory release*/
void trie_destroy(trie_node *node) {
    int i;
    if(node == NULL)
        return;
    for(i = 0; i < N; i++) {
        trie_destroy(node->children[i]);
    }
    free(node->end_of_str);
    free(node);
}

int main() {
    int i;
    trie_node *root;
    const char *f;
    const char *strings[4] = {
        [0] = "hello",
        [1] = "hellow",
        [2] = "hellno",
        [3] = "mika"
    };
    root = make_trienode();
    for(i=0;i<4;i++) {
        trie_insert(root, strings[i], &strings[i]);
    }
    for(i=0;i<4;i++) {
        f = search_trie(root, (char *)strings);
        if(f) {
            printf("%s",f);
        }
    }
    trie_destroy(root);
	return 1;
}