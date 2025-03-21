#ifndef BUILD_H
#define BUILD_H

#include "init.h"

void build_graph(DependencyGraph* dg);
void print_dependencies(const DependencyGraph* dg);
void cleanup(DependencyGraph* dg);

#endif
