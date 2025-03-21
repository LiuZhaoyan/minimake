#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_line(char* line, DependencyGraph* dg) {
	line[strcspn(line,"\n")]='\0';
	// 去除注释
	char* comment = strchr(line, '#');
	if (comment) *comment = '\0';

	// 是否为目标行
	char* colon = strchr(line, ':');
	if (!colon) return;
	
	char *target=strtok(line,":");
	// 去除空白
	target = strtok(target, " \t");
	if (!target) return;

	// 添加目标到列表
	if (dg->target_count >= MAX_TARGETS) {
		fprintf(stderr, "Too many targets\n");
		exit(EXIT_FAILURE);
	}

	Target* t = &dg->targets[dg->target_count++];
	strncpy(t->name, target, sizeof(t->name)-1);
	t->deps = malloc(MAX_DEPS * sizeof(char*));
	t->dep_count = 0;

	// 处理依赖项
	char* dep;
	while ((dep = strtok(NULL, " \t\n")) != NULL) {
		//if (*dep == '\0') continue;

		if (t->dep_count >= MAX_DEPS) {
			fprintf(stderr, "Too many dependencies for %s\n", target);
			exit(EXIT_FAILURE);
		}

		t->deps[t->dep_count++] = strdup(dep);

		// 确保依赖项在顶点表中
		get_vertex_id(&dg->vertices, dep);
	}

	// 确保目标在顶点表中
	get_vertex_id(&dg->vertices, target);
}

