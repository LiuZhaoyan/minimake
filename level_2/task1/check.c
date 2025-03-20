#include "s_utils.h"
#include "store.h"
#include "check.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//报错生成报错并储存
void add_error(ErrorInfo **errors,int *error_count,ErrorType type, int line_number, const char *target, const char *name){
    *errors=realloc(*errors,((*error_count)+1)*sizeof(ErrorInfo));
    ErrorInfo *err = &(*errors)[*error_count];
    //添加报错信息
    err->type = type;
    err->line_number = line_number;
    if (target) {
        strncpy(err->target, target, MAX_FILE_NAME_LEN);
        err->target[MAX_FILE_NAME_LEN] = '\0';
    } else {
        err->target[0] = '\0';
    }
    if (name) {
        strncpy(err->name, name, MAX_FILE_NAME_LEN);
        err->name[MAX_FILE_NAME_LEN] = '\0';
    } else {
        err->name[0] = '\0';
    }
    (*error_count)++;
}

//目标是否存在
int target_exist(const char *target, TargetBlock *targets,int target_count){
    for(int i=0;i<target_count;i++){
        if(strcmp(target,(targets+i)->target)==0) {return 1;}
    }
    return 0;
}

//检查依赖: 1.依赖是否在当前目录 2.依赖是否在makefile文件内
void check_dependencies(ErrorInfo **errors,TargetBlock *targets,int target_count,int *error_count) {
    for(int i=0;i<target_count;i++){
        TargetBlock *t=targets+i;
        //检查每个目标块
        for(int j=0;j<t->dep_count;j++){
            char *dep=t->dependencies[j];
            //检查是否在当前目录中
            if(access(dep,F_OK)==0){continue;}
            //检查是否在文件中
            int found=0;
            for(int k=0;k<target_count;k++){
                if(strcmp(dep,(targets+k)->target)==0){
                    found =1;
                    break;
                }
            }
            if(!found){
                add_error(errors,error_count,ERR_INVALID_DEPENDENCY,t->line_number,t->target,dep);
            }
        }
    }
}

//打印报错
void print_errors(ErrorInfo **errors,int error_count){
    for (int i = 0; i < error_count; i++) {
        ErrorInfo *err = &(*errors)[i];
        switch (err->type) {
            case ERR_DUPLICATE_TARGET:
                printf("Duplicate target definition '%s'\n",err->target);
                break;
            case ERR_INVALID_DEPENDENCY:
                printf("Invalid dependency '%s'\n",err->name);
                break;
        }
    }
}


