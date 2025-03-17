#include "s_utils.h"
#include <ctype.h>
#include <string.h>

int is_blank_line(char *line){
    while(*line !='\0'){
        if(!isspace((unsigned char)*line)){return 0;}
        line++;
    }
    return 1;
}

void remove_trailing_whitespace(char *line){
    if(!line) return;
    int len=strlen(line);
    while(len>0&&isspace((unsigned char)*line)==0){len--;}
    line[len]='\0';
}

void remove_comment(char *line){
    char *flag=strchr(line,'#');
    if(flag){*flag='\0';}
}

