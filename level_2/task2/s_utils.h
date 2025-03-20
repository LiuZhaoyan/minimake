#ifndef S_UTILS_H
#define S_UTILS_H

void trim(char *str);
void remove_trailing_whitespace(char *line);
void remove_comment(char *line);
//检查函数用const比较严谨
int is_blank_line(const char *line);
int is_target_line(const char *line);

#endif
