#ifndef _TRIE_H_
#define _TRIE_H_

typedef struct trie_node * trie_node;

trie_node* make_trie(void)
void trie_insert(Trie trie,const char *string,void * end_of_str_ctx);
void * trie_exists(Trie trie,const char *string);
void trie_destroy(Trie *trie);
