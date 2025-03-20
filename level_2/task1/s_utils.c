#include "s_utils.h"
#include <string.h>
#include <ctype.h>

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
