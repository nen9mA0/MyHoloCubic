#include "script_utils.h"

const char delimiter = '/';

char *path_join(char *dir, char *name)
{
    if(!dir || !name)
        return NULL;

    int len_dir = strlen(dir);
    int len_name = strlen(name);
    char *p = malloc(len_dir+len_name+2);
    strcpy(p, dir, len_dir);
    int i = p+len_dir;
    while(p[i] == delimiter)
        i--;
    p[++i] = delimiter;
    strcpy(&p[++i], name, len_name);
    p[i+len_name] = '\0';
    return p;
}

int mystrcmp(char *a, char*b)
{
    if(!a || !b)
    {
        return -1;
    }
    return strcmp(a, b);
}

char* mydirname(char* path)
{
    int len;
    char *ret = NULL;
    char *p = strrchr(path, delimiter);
    if(p)
    {
        len = p-path+1;
        ret = malloc(len);
        memcpy(ret, path, len-1);
        ret[len-1] = '\0';
    }
    return ret;
}

char* mybasename(char* path)
{
    int len;
    char *p = strrchr(path, delimiter);
    return p;
}