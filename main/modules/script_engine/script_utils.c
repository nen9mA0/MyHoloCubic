#include "script_utils.h"

char* dirname(char* path)
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

char* basename(char* path)
{
    int len;
    char *p = strrchr(path, delimiter);
    return p;
}