#include "s_utils.h"
#include <string.h>
#include <ctype.h>

//去空格
void trim(char *str) {
    str[strcspn(str,"\n")]='\0';

    if (str == NULL) return;
    //去头
    int begin = 0;
    while (isspace((unsigned char)str[begin])) begin++;
    //去尾
    int end = strlen(str) - 1;
    while (end >= begin && isspace((unsigned char)str[end])) end--;

    for (int i = begin; i <= end; i++) {
        str[i - begin] = str[i];
    }
    str[end - begin + 1] = '\0';
}

// 去除行尾空白字符
void remove_trailing_whitespace(char *line) {
    //顺便去掉换行符
    line[strcspn(line,"\n")]='\0';

    if (line == NULL) return;
    int len = strlen(line);
    while (len > 0 && isspace((unsigned char)line[len - 1])) {
        len--;
    }
    line[len] = '\0';
}
// 移除注释
void remove_comment(char *line) {
    char *pound = strchr(line, '#');
    if (pound != NULL) {*pound = '\0';}
}

//检查是否为空行
int is_blank_line(const char *line) {
    while (*line != '\0') {
        if (!isspace((unsigned char)*line)) {
            return 0;
        }
        line++;
    }
    return 1;
} 
int is_target_line(const char *line){
    if(strchr(line,':')){return 1;}
    return 0;
}
