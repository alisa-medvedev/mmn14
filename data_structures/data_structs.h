#ifndef _TRIE_H_
#define _TRIE_H_


typedef struct trie * Trie;

void trie_insert(Trie trie,const char *string,void * end_of_str_ctx);
int trie_exists(Trie trie,const char *string);
void trie_delete(Trie trie,const void *string);
void trie_destroy(Trie *trie);
