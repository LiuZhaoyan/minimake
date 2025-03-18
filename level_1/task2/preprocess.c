#include "makefile_process.h"
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[]){
    int verbose=(argc>1 && strcmp(argv[1],"--verbose")==0);
    process_makefile(verbose);
    return EXIT_SUCCESS;
}
