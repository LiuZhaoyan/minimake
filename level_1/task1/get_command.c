#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_help(){
    printf("Usage:\n");
    printf("Options:\n");
    printf("--help    Display help message\n");
}

int main(int argc,char *argv[]){
    //如果没有提供参数
    if(argc==1){
        printf("Missing parameters\n");
        return 1;
    }
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"--help")==0){
            print_help();
        }else{
            printf("Error: Unknown option '%s'\n",argv[i]);
            return 1;
        }

    }
    return 0;
}
