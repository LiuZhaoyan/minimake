#include "makefile_process.h"
#include "s_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_makefile(int verbose){
    FILE *input=fopen("./Makefile","r");
    if(!input){
        perror("Makefile not found\n");
        exit(EXIT_FAILURE);
    }
    FILE *output=NULL;
    if(verbose){
        output=fopen("Minimake_cleared.mk","w");
        if(!output){
            perror("Error\n");
            exit(EXIT_FAILURE);
        }
    }
    char line[LINE_MAX];
    while(fgets(line,sizeof(line),input)){
        if(is_blank_line(line)){continue;}
        remove_trailing_whitespace(line);
        remove_comment(line);
        if(verbose){
            fprintf(output,"%s\n",line);
        }else{
            printf("%s\n",line);
        }

    }
    fclose(input);
    if(output){fclose(output);}
}


