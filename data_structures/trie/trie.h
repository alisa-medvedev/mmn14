
typedef struct trie_node * trie_node;

trie_node *make_trienode() 
const char *trie_insert(trie_node *root,const char *string,void *end_of_str)
void *search_trie(trie_node *root,const char *string)
void trie_destroy(trie_node *node)
