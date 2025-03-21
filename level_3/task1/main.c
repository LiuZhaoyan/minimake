#include "init.h"
#include "parser.h"
#include "build.h"
#include <igraph/igraph.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char *argv[]){
	if(argc!=2){
		fprintf(stderr,"Usage: %s <makefile>\n",argv[0]);
	}
	
	DependencyGraph dg={0};
	dg.targets=malloc(MAX_TARGETS * sizeof(Target));
	init_vertex_map(&dg.vertices);

	FILE *makefile = fopen(argv[1],"r");
	if(!makefile){
		perror("Eror opening file");
		cleanup(&dg);
		exit(EXIT_FAILURE);
	}

	char line[MAX_LINE];
	while(fgets(line,sizeof(line),makefile)){
		parse_line(line,&dg);
	}
	fclose(makefile);

	build_graph(&dg);
	print_dependencies(&dg);
	cleanup(&dg);
	return 0;
}
