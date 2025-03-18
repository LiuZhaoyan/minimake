#include "check_grammar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
void check_grammar() {

    FILE *file = fopen("./Makefile", "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[LINE_MAX];
    int line_number = 0;
    int in_target = 0; // 标记是否在目标块中

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        // 去掉行末的换行符
        line[strcspn(line, "\n")] = '\0';
        //使用tasks的程序去掉空行和注释

        if(!isspace(line[0]) && line[0]!='\t'){
            //进入目标
            in_target = 1;
            // 检查目标行
            if (strchr(line, ':') != NULL) {
                printf("Line%d: Missing colon in target definition",line_number);
            }
        }
        //若出现命令前没有目标的情况(据题意:只有第一行有可能出现这种情况)
        if(line_number==1 && in_target==0){
            printf("Line%d: Command found before rule",line_number);
        }

        // 检查命令行的Tab键
        if (in_target) {

            if (line[0] != '\t') {
                printf("Line %d: Command must start with a tab\n", line_number);
            }
        }
    }

    fclose(file);

}

