#ifndef PARSER_H
#define PARSER_H

#define MAX_FILE_NAME_LEN 32
#define MAX_LINE_LEN 256

typedef struct{
    char target[MAX_FILE_NAME_LEN+1];
    char **dependencies;
    char command[MAX_LINE_LEN];
    int dep_count;
    int line_number;
}TargetBlock;

typedef enum {
    ERR_DUPLICATE_TARGET,
    ERR_INVALID_DEPENDENCY,
}ErrorType;

typedef struct{
    ErrorType type;
    char target[MAX_FILE_NAME_LEN+1];
    char name[MAX_FILE_NAME_LEN+1];
    int line_number;
}ErrorInfo;

void parse_target_line(char *line, int line_number, TargetBlock *block,const char *target);
void get_command(char *line,TargetBlock *block);

#endif
