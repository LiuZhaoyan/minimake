#include "s_utils.h"
#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//修改task1的函数parse_target_line,只有一个target的情况
void parse_target_line(char *line, int line_number, TargetBlock *block,const char *target){
    char *colon=strchr(line,':');
    *colon='\0';

    //储存目标
    strncpy(block->target,target, MAX_FILE_NAME_LEN);
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
//有command的情况下获取command
void get_command(char *line,TargetBlock *block){
    line[strcspn(line, "\n")] = '\0';
    remove_trailing_whitespace(line);
    if (line[0] == '\t') {
        strcpy(block->command, line + 1);//可能需要strncpy
    } else {
        fprintf(stderr, "Command for target '%s' must start with tab\n", block->target);
    }
}
