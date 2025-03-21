#ifndef INIT_H
#define INIT_H

#define MAX_LINE 256
#define MAX_TARGETS 100
#define MAX_DEPS 20

#include <igraph/igraph.h>
typedef struct {
	char name[50];
	char** deps;
	int dep_count;
} Target;

typedef struct {
	char** names;
	int count;
} VertexMap;

//不把编号写在Target中, 单独管理编号
typedef struct {
	Target* targets;
	int target_count;
	VertexMap vertices;
	igraph_t graph;
	int* in_degrees;
} DependencyGraph;

void init_vertex_map(VertexMap* vm);
int get_vertex_id(VertexMap* vm, const char* name);
#endif
