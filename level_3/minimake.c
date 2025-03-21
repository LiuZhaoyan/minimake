#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <igraph/igraph.h>

#define MAX_LINE 256
#define MAX_TARGETS 100
#define MAX_DEPS 20

typedef struct {
	char name[50];
	char** deps;
	int dep_count;
} Target;

typedef struct {
	char** names;
	int count;
} VertexMap;

typedef struct {
	Target* targets;
	int target_count;
	VertexMap vertices;
	igraph_t graph;
	int* in_degrees;
} DependencyGraph;

/* 初始化顶点名称映射表 */
void init_vertex_map(VertexMap* vm) {
	vm->names = malloc(MAX_TARGETS * sizeof(char*));
	vm->count = 0;
}

/* 查找或添加顶点 */
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

/* 解析Makefile行 */
void parse_line(const char* line, DependencyGraph* dg) {
	char buffer[MAX_LINE];
	strcpy(buffer, line);

	// 去除注释
	char* comment = strchr(buffer, '#');
	if (comment) *comment = '\0';

	// 分割目标与依赖
	char* target = strtok(buffer, ":");
	if (!target) return;

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
		if (*dep == '\0') continue;

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

/* 构建依赖图 */
void build_graph(DependencyGraph* dg) {
	// 创建顶点
	igraph_empty(&dg->graph, dg->vertices.count, IGRAPH_DIRECTED);

	// 创建边列表
	igraph_vector_int_t edges;
	igraph_vector_int_init(&edges, 0);

	// 遍历所有目标
	for (int i = 0; i < dg->target_count; i++) {
		Target t = dg->targets[i];
		int target_id = get_vertex_id(&dg->vertices, t.name);

		// 添加依赖边
		for (int j = 0; j < t.dep_count; j++) {
			int dep_id = get_vertex_id(&dg->vertices, t.deps[j]);

			igraph_vector_int_push_back(&edges, dep_id);
			igraph_vector_int_push_back(&edges, target_id);
		}
	}

	// 添加边到图
	igraph_add_edges(&dg->graph, &edges, 0);
	igraph_vector_int_destroy(&edges);

	// 计算入度
	dg->in_degrees = calloc(dg->vertices.count, sizeof(int));
	igraph_vector_int_t degrees;
	igraph_vector_int_init(&degrees, dg->vertices.count);
	igraph_degree(&dg->graph, &degrees, igraph_vss_all(), IGRAPH_IN, IGRAPH_LOOPS_SW);

	for (int i = 0; i < dg->vertices.count; i++) {
		dg->in_degrees[i] = VECTOR(degrees)[i];
	}
	igraph_vector_int_destroy(&degrees);
}

/* 打印依赖关系 */
void print_dependencies(const DependencyGraph* dg) {
	printf("Dependency Graph:\n");
	printf("Total Vertices: %d\n", dg->vertices.count);

	for (int i = 0; i < dg->vertices.count; i++) {
		printf("[%d] %s (in-degree: %d)\n", 
				i, dg->vertices.names[i], dg->in_degrees[i]);
	}

	printf("\nEdges:\n");
	igraph_es_t es;
	igraph_eit_t it;
	igraph_es_all(&es, IGRAPH_EDGEORDER_ID);
	igraph_eit_create(&dg->graph, es, &it);

	while (!IGRAPH_EIT_END(it)) {
		igraph_integer_t edge = IGRAPH_EIT_GET(it);
		igraph_integer_t from, to;
		igraph_edge(&dg->graph, edge, &from, &to);

		printf("%s -> %s\n", 
				dg->vertices.names[from],
				dg->vertices.names[to]);
		IGRAPH_EIT_NEXT(it);
	}

	igraph_es_destroy(&es);
	igraph_eit_destroy(&it);
}

/* 释放资源 */
void cleanup(DependencyGraph* dg) {
	// 释放顶点名称
	for (int i = 0; i < dg->vertices.count; i++) {
		free(dg->vertices.names[i]);
	}
	free(dg->vertices.names);

	// 释放目标依赖
	for (int i = 0; i < dg->target_count; i++) {
		for (int j = 0; j < dg->targets[i].dep_count; j++) {
			free(dg->targets[i].deps[j]);
		}
		free(dg->targets[i].deps);
	}
	free(dg->targets);

	// 释放图资源
	igraph_destroy(&dg->graph);
	free(dg->in_degrees);
}

int main(int argc, char** argv) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <makefile>\n", argv[0]);
		return EXIT_FAILURE;
	}

	DependencyGraph dg = {0};
	dg.targets = malloc(MAX_TARGETS * sizeof(Target));
	init_vertex_map(&dg.vertices);

	FILE* fp = fopen(argv[1], "r");
	if (!fp) {
		perror("Error opening file");
		cleanup(&dg);
		return EXIT_FAILURE;
	}

	char line[MAX_LINE];
	while (fgets(line, sizeof(line), fp)) {
		parse_line(line, &dg);
	}
	fclose(fp);

	build_graph(&dg);
	print_dependencies(&dg);
	cleanup(&dg);

	return EXIT_SUCCESS;
}
