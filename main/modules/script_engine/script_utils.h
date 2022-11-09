#ifndef _SCRIPT_UTILS_H
#define _SCRIPT_UTILS_H

#include "script_engine.h"

const char delimiter = '/';

char* dirname(char* path);  // use malloc, return a new chunk, which should be freed after used
char* basename(char* path);

#endif