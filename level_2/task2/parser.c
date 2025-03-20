#include "s_utils.h"
#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void get_dependencies(TargetBlock *block,char *dep_str){
    block->dep_count = 0;
    block->dependencies = NULL;

    char *saveptr;
    char *dep = strtok_r(dep_str, " ", &saveptr);

    while (dep != NULL) {
        trim(dep);

        block->dependencies = realloc(block->dependencies, (block->dep_count + 1) * sizeof(char *));
        block->dependencies[block->dep_count] = strdup(dep);
        block->dep_count++;

        dep = strtok_r(NULL, " ", &saveptr);
    }
}

int get_command(char *line,TargetBlock *block)
{
    if(line[0]!='\t'){
        fprintf(stderr,"Error: Command:%s\n without Tab!",line);
    }else {
        strncpy(block->command,line+1,MAX_LINE_LEN);
        return 1;
    }
    return 0;
}

