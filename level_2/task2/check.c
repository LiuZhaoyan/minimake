#include "check.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int check_dependencies(TargetBlock *block){
    for(int i=0;i<block->dep_count;i++){
        if(access(block->dependencies[i],F_OK)!=0){
            fprintf(stderr,"Invalid dependency");
            return 0;
        }
    }
    return 1;
}
