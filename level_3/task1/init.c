#include "init.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_vertex_map(VertexMap* vm) {
	vm->names = malloc(MAX_TARGETS * sizeof(char*));
	vm->count = 0;
}
int get_vertex_id(VertexMap* vm, const char* name) {
	// 检查是否已存在
	for (int i = 0; i < vm->count; i++) {
		if (strcmp(vm->names[i], name) == 0) {
			return i;
		}
	}

	// 添加新顶点
	if (vm->count >= MAX_TARGETS) {
		fprintf(stderr, "Max vertices reached\n");
		exit(EXIT_FAILURE);
	}

	vm->names[vm->count] = strdup(name);
	return vm->count++;
}




































