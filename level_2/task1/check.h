#ifndef CHECK_H
#define CHECK_H

#include "store.h"
void add_error(ErrorInfo **errors,int *error_count,ErrorType type,int line_number,const char *target,const char *name);

int target_exist(const char *target,TargetBlock *targets,int target_count);
void check_dependencies(ErrorInfo **errors,TargetBlock *targets,int target_count,int *error_count);

void print_errors(ErrorInfo **errors,int error_count);

#endif
