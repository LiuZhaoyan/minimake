#include "s_utils.h"
#include "parser.h"
#include "check.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//储存目标
TargetBlock *targets = NULL;
int target_count = 0;
//储存报错
ErrorInfo *errors = NULL;
int error_count = 0;

int main(int argc,char *argv[]){
    if (argc < 2) {
        fprintf(stderr, "Usage: %s target\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const char *target = argv[1];

    FILE *makefile=fopen("./Makefile","r");
    if(!makefile){
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int found=0;
    int line_num=0;
    char line[MAX_LINE_LEN];

    char cmd[1024] = {0};

    while(fgets(line,sizeof(line),makefile)!=NULL){
        line_num++;
        if(is_blank_line(line)){continue;}
        remove_comment(line);

        if (is_target_line(line)) {
            found=1;
            TargetBlock block;
            parse_target_line(line, line_num, &block,target);
            targets = realloc(targets, (target_count + 1) * sizeof(TargetBlock));
            targets[target_count] = block;
            target_count++;
        }
        if (fgets(line, sizeof(line), makefile)) {
            line_num++;
            get_command(line,targets+target_count);
        } else {
            fprintf(stderr, "Missing command for target '%s'\n", target);
            exit(EXIT_FAILURE);//是否要写,存疑
        } 

    }
    //如果没有目标
    if (!found) {
        fprintf(stderr, "Target '%s' not found\n", target);
    }
    //检查依赖
    check_dependencies(&errors, targets, target_count, &error_count);

    //只有一个目标的情况下
    if(errors==NULL){
        for(int i=0;i<target_count;i++){
            strcpy(cmd,targets[i].target);
            printf("Executing: %s\n", cmd);
            int ret = system(cmd);
            if (ret!= 0) {
                fprintf(stderr, "Command failed with code %d\n", ret);
            }
        }
    } else{
        print_errors(&errors,error_count);   
    }
    //释放内存
    for (int i = 0; i < target_count; i++) {
        TargetBlock *t = &targets[i];
        for (int j = 0; j < t->dep_count; j++) {
            free(t->dependencies[j]);
        }
        free(t->dependencies);
    }
    free(targets);
    free(errors);
    fclose(makefile);
    return 0;
}
