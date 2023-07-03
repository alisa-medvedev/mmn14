#include "trie.h"

int main() {
    int i;
    Trie t;
    const char *f;
    const char *strings[4] = {
        [0] = "hello",
        [1] = "hellow",
        [2] = "hellno",
        [3] = "mika"
    };
    t = trie();
    for(i=0;i<4;i++) {
        trie_insert(trie: t, string: strings[i], end_of_str_ctx: &strings[i]);
    }
    for(i=0;i<4;i++) {
        f = trie_exists(Trie trie, (char *)string);
        if(f) {
            printf("%s",f);
        }
    }
}