#include <stdio.h>
#include "preprocess.h"

int main() {
    FILE* as_file;
    FILE* am_file;
    open_macros("test.txt", &as_file, &am_file);


    fclose(as_file);
    fclose(am_file);
    return 0;
}