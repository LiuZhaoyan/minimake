#include "s_utils.h"
#include "store.h"
#include "check.h"
#include <stdio.h>
#include <stdlib.h>

//储存目标
TargetBlock *targets = NULL;
int target_count = 0;
//储存报错
ErrorInfo *errors = NULL;
int error_count = 0;

int main(){
    FILE *input=fopen("./Makefile","r");
    if(!input){
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int line_num=0;
    char line[MAX_LINE_LEN];
    while(fgets(line,sizeof(line),input)!=NULL){
        line_num++;
        if(is_blank_line(line)){continue;}
        if (is_target_line(line)) {
            TargetBlock block;
            parse_target_line(line, line_num, &block);
            //判断目标是否已出现
            if (target_exist(block.target,targets,target_count)) {
                add_error(&errors,&error_count,ERR_DUPLICATE_TARGET, line_num, block.target, block.target);
            } else {
                targets = realloc(targets, (target_count + 1) * sizeof(TargetBlock));
                targets[target_count] = block;
                target_count++;
            }
        }

    }
    check_dependencies(&errors, targets, target_count, &error_count);
    print_errors(&errors,error_count);
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
    return 0;
}
