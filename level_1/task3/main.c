#include "check_grammar.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc,char *argv[]){
    FILE *file=fopen(argv[1],"r");
    check_grammar(file);
    fclose(file);
    return EXIT_SUCCESS;
}
