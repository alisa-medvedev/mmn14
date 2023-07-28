#include "base.h"

void skip_spaces(char *line) {
    int i = 0;
    while (line[i] == ' ' || line[i] == '\t') {
        i++;
    }
}