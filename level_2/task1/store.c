#include "s_utils.h"
#include "store.h"
#include <string.h>
#include <stdlib.h>

void parse_target_line(char *line, int line_number, TargetBlock *block){
    char *colon=strchr(line,':');
    *colon='\0';

    //储存目标
    char *target_part = line;
    remove_trailing_whitespace(target_part);
    strncpy(block->target, target_part, MAX_FILE_NAME_LEN);
    block->target[MAX_FILE_NAME_LEN]='\0';//避免长度过长

    //储存依赖
    int dep_count = 0;
    block->dependencies = NULL;

    char *dep_part=colon+1;
    char *saveptr;
    char *dep=strtok_r(dep_part," ",&saveptr);

    while(dep){
        remove_trailing_whitespace(dep);
        block->dependencies=realloc(block->dependencies,(block->dep_count+1)*sizeof(char *));
        block->dependencies[dep_count]=strdup(dep);
        block->dep_count++;
        
        dep=strtok_r(NULL," ",&saveptr);
    }
    block->line_number = line_number;
}
