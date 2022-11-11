#ifndef _SCRIPT_UTILS_H
#define _SCRIPT_UTILS_H

#include <string.h>
#include <stdlib.h>

char *path_join(char *dir, char *name); // use malloc
int mystrcmp(char *a, char*b);
char* mydirname(char* path);  // use malloc, return a new chunk, which should be freed after used
char* mybasename(char* path);

#endif