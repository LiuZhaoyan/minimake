#include "s_utils.h"
#include "parser.h"
#include "check.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
//检查减少版
int main(int argc,char *argv[]){
    if (argc < 2) {
        fprintf(stderr, "Usage: %s target\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const char *target = argv[1];

    FILE *makefile = fopen("./Makefile", "r");
    if (!makefile) {
        perror("Error opening Makefile");
        exit(EXIT_FAILURE);
    }

    char line[256];
    int found = 0;
    char cmd[MAX_LINE_LEN] = {0};
    TargetBlock block;

    while(fgets(line,sizeof(line),makefile)){
        if(is_blank_line(line)){continue;}
        remove_comment(line);
        trim(line);

        if (is_target_line(line)) {
            found=1;
            //获得target
            trim(line);
            char *colon=strchr(line,':');
            *colon = '\0';

            char *dep_str = colon + 1;
            trim(dep_str);
            //获得依赖
            get_dependencies(&block,dep_str);
            //获得一个command
            if (fgets(line, sizeof(line), makefile)) {
                if(get_command(line,&block)){
                    strcpy(cmd,block.command);
					trim(cmd);
                }else {
					goto cleanup;
				}
            } else {
                fprintf(stderr, "Missing command for target '%s'\n", target);
                goto cleanup;
            }

        }
    }

    if (!found) {
        fprintf(stderr, "Target in Makefile not found\n");
        goto cleanup;
    }
    strncpy(block.target,target,MAX_FILE_NAME_LEN);

    if(check_dependencies(&block)==0 ){
		goto cleanup;
	}
    //输出命令
    printf("Exicuting: %s\n",cmd);
    int ret = system(cmd);
    if (ret != 0) {
        fprintf(stderr, "Command failed with code %d\n", ret);
        goto cleanup;
    }
cleanup:
	printf("Begin to clean\n");
    //释放内存
    for(int i=0;i<block.dep_count;i++){
        free((block.dependencies)[i]);
    }
    free(block.dependencies);

    fclose(makefile);
    return found? 0:1;
}
