#ifndef PARSER_H
#define PARSER_H

#define MAX_FILE_NAME_LEN 32
#define MAX_LINE_LEN 512
typedef struct{
    char target[MAX_FILE_NAME_LEN+1];
    char **dependencies;
    char command[MAX_LINE_LEN];
    int dep_count;
}TargetBlock;

void get_dependencies(TargetBlock *block,char *dep_str);
int get_command(char *line,TargetBlock *block);

#endif
