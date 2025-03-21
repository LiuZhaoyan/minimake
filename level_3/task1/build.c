#include "build.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
void build_graph(DependencyGraph *dg){
	//创建顶点
	igraph_empty(&dg->graph,dg->vertices.count,IGRAPH_DIRECTED);

	igraph_vector_int_t edges;
	igraph_vector_int_init(&edges,0);

	for(int i=0;i<dg->target_count;i++){
		Target t= dg->targets[i];
		int target_id=get_vertex_id(&dg->vertices,t.name);

		//添加依赖边
		for(int j=0;j<t.dep_count;j++){
			//!!查询依赖是否为顶点,并添加依赖为顶点
			int dep_id=get_vertex_id(&dg->vertices,t.deps[j]);

			//添加边,dep为起点 target为终点
			igraph_vector_int_push_back(&edges,target_id);
			igraph_vector_int_push_back(&edges,dep_id);

		}
	}

	igraph_add_edges(&dg->graph,&edges,0);
	igraph_vector_int_destroy(&edges);

	//添加入度, in_degrees数组编号与顶点表对应
	dg->in_degrees = calloc(dg->vertices.count, sizeof(int));
	igraph_vector_int_t degrees;
	igraph_vector_int_init(&degrees,dg->vertices.count);
	igraph_degree(&dg->graph,&degrees,igraph_vss_all(),IGRAPH_IN,IGRAPH_LOOPS_SW);
	//添加到图中
	for(int i=0;i<dg->vertices.count;i++){
		dg->in_degrees[i]= VECTOR(degrees)[i];
	}

	igraph_vector_int_destroy(&degrees);

}


//打印与释放
void print_dependencies(const DependencyGraph* dg) {
	printf("Graph:\n");
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
